#define _DEFAULT_SOURCE

#include <dirent.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "fs.h"
#include "state.h"

void free_list(EntryList *list)
{
    EntryNode *current = list->head;
    while (current != NULL)
    {
        EntryNode *next = current->next;
        free(current);
        current = next;
    }
}

void add_entry_node(char *name, EntryType type, EntryList *list)
{
    EntryNode *node = malloc(sizeof(EntryNode));
    if (node == NULL)
    {
        free_list(list);
        return;
    }
    node->name = name;
    node->type = type;
    node->next = NULL;
    node->prev = NULL;

    // If list is empty
    if (list->head == NULL)
    {
        list->head = list->tail = list->cursor = node;
        return;
    }

    EntryNode *current = list->head;
    while (current != NULL)
    {
        bool insert_before = FALSE;

        // Sort directory before file
        if (node->type == ENTRY_DIR && current->type == ENTRY_FILE)
        {
            insert_before = TRUE;
        }

        // If same type then sort in alphabetical order
        else if (node->type == current->type && strcmp(node->name, current->name) < 0)
        {
            insert_before = TRUE;
        }

        if (insert_before)
        {
            node->next = current;
            node->prev = current->prev;

            if (current->prev == NULL)
            {
                current->prev = node;
                list->head = node;
                list->cursor = node;
                return;
            }
            current->prev->next = node;
            current->prev = node;
            return;
        }
        current = current->next;
    }
    node->prev = list->tail;
    list->tail->next = node;
    list->tail = node;
}

void list_dir(AppState *state, EntryList *list)
{
    // Initialize the doubly linked list
    free_list(list);
    list->head = NULL;
    list->tail = NULL;

    DIR *dir = opendir(state->dir_path);
    if (dir == NULL)
    {
        clear();
        mvprintw(0, 0, "Directory cannot be openned.");
        return;
    }

    struct dirent *dir_entry;
    while ((dir_entry = readdir(dir)) != NULL)
    {
        if (strcmp(dir_entry->d_name, ".") == 0 || strcmp(dir_entry->d_name, "..") == 0)
        {
            continue;
        }

        EntryType type;
        if (dir_entry->d_type == DT_DIR)
        {
            type = ENTRY_DIR;
        }
        else if (dir_entry->d_type == DT_REG)
        {
            type = ENTRY_FILE;
        }
        add_entry_node(dir_entry->d_name, type, list);
    }

    closedir(dir);
}

void free_stack(Stack *stack)
{
    StackNode *current_stack_node = stack->top;
    while (current_stack_node != NULL)
    {
        StackNode *next = current_stack_node->next;
        free(current_stack_node);
        current_stack_node = next;
    }
}

void subdir_stack_push(AppState *state, Stack *stack)
{
    StackNode *stack_node = malloc(sizeof(StackNode));
    if (stack_node == NULL)
    {
        return;
    }
    stack_node->path = strdup(state->dir_path);
    stack_node->next = stack->top;

    stack->top = stack_node;
}

void subdir_stack_pop(AppState *state, Stack *stack)
{
    if (stack->top == NULL)
    {
        return;
    }

    StackNode *stack_node = stack->top;
    stack->top = stack_node->next;

    if (stack->top != NULL)
    {
        free(state->dir_path);
        state->dir_path = strdup(stack->top->path);
    }
    else
    {
        free(state->dir_path);
        state->dir_path = strdup("..");
    }

    free(stack_node);
}
