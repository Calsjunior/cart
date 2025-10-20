#define _DEFAULT_SOURCE

#include <dirent.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "fs.h"
#include "state.h"

static void add_entry_node(char *name, EntryType type, EntryList *list);

void free_list(EntryList *list)
{
    EntryNode *current = list->head;
    while (current != NULL)
    {
        EntryNode *next = current->next;
        free(current);
        current = next;
    }
    list->head = NULL;
    list->tail = NULL;
    list->cursor = NULL;
    list->count_entries = 0;
    list->scroll_offset = 0;
}

static void add_entry_node(char *name, EntryType type, EntryList *list)
{
    EntryNode *node = malloc(sizeof(EntryNode));
    if (node == NULL)
    {
        free_list(list);
        return;
    }
    node->name = strdup(name);
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
        else if (dir_entry->d_type == DT_UNKNOWN)
        {
            continue;
        }
        add_entry_node(dir_entry->d_name, type, list);
        list->count_entries++;
    }

    closedir(dir);
}

void free_stack(Stack *stack)
{
    StackNode *current_stack_node = stack->top;
    while (current_stack_node != NULL)
    {
        StackNode *next = current_stack_node->next;
        free(current_stack_node->path);
        free(current_stack_node->cursor_name);
        free(current_stack_node);
        current_stack_node = next;
    }
    stack->top = NULL;
}

void subdir_stack_push(AppState *state, Stack *stack, EntryList *list)
{
    StackNode *stack_node = malloc(sizeof(StackNode));
    if (stack_node == NULL)
    {
        return;
    }

    // Push entered directory's name onto the stack
    if (list->cursor != NULL)
    {
        stack_node->cursor_name = strdup(list->cursor->name);
    }

    stack_node->path = strdup(state->dir_path);
    stack_node->scroll_offset = list->scroll_offset;
    stack_node->next = stack->top;
    stack->top = stack_node;
}

void subdir_stack_pop(AppState *state, Stack *stack, EntryList *list)
{
    if (stack->top == NULL)
    {
        return;
    }

    StackNode *stack_node = stack->top;
    stack->top = stack_node->next;

    free(state->dir_path);
    state->dir_path = strdup(stack_node->path);
}

void stack_restore_cursor(Stack *stack, EntryList *list)
{
    if (stack->top == NULL)
    {
        return;
    }

    StackNode *stack_node = stack->top;

    list->scroll_offset = stack_node->scroll_offset;

    // Check if new directory list's name matches name that is pushed to the stack
    for (EntryNode *current = list->head; current != NULL; current = current->next)
    {
        if (strcmp(current->name, stack_node->cursor_name) == 0)
        {
            list->cursor = current;
            break;
        }
    }
}

void navigate_subdir(AppState *state, EntryList *list)
{
    char new_path[PATH_MAX];
    if (strcmp(state->dir_path, "/") == 0)
    {
        snprintf(new_path, sizeof(new_path), "/%s", list->cursor->name);
    }
    else
    {
        snprintf(new_path, sizeof(new_path), "%s/%s", state->dir_path, list->cursor->name);
    }

    free(state->dir_path);
    state->dir_path = strdup(new_path);
}

void navigate_root(AppState *state)
{
    char *slash = strrchr(state->dir_path, '/');
    if (slash != NULL && slash != state->dir_path)
    {
        *slash = '\0';
        return;
    }
    strcpy(state->dir_path, "/");
}
