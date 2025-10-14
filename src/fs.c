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

            // Assign the next pointer of node that the current previous is pointing to
            if (current->prev != NULL)
            {
                current->prev->next = node;
            }
            current->prev = node;

            if (node->prev == NULL)
            {
                list->head = node;
                list->cursor = node;
            }

            if (node->next == NULL)
            {
                list->tail = node;
            }
            return;
        }
        node->next = current->next;
        node->prev = current;
        list->tail = node;
        current = current->next;
    }
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
