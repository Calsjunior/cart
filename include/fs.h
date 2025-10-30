#ifndef FS_H
#define FS_H

#include <stdlib.h>

#include "state.h"

#define PATH_MAX 4096

// Navigating in the current working directory
typedef enum
{
    ENTRY_DIR,
    ENTRY_FILE,
    ENTRY_SYMLINK_DIR,
    ENTRY_SYMLINK_FILE,
} EntryType;

typedef struct EntryNode
{
    char *name;
    EntryType type;
    struct EntryNode *next;
    struct EntryNode *prev;
} EntryNode;

typedef struct
{
    EntryNode *head;
    EntryNode *tail;
    EntryNode *cursor;
    int count_entries;
    int scroll_offset;
} EntryList;

// Navigating into and out of sub directories
typedef struct StackNode
{
    char *path;
    char *cursor_name;
    int scroll_offset;
    struct StackNode *next;
} StackNode;

typedef struct Stack
{
    StackNode *top;
} Stack;

void list_dir(AppState *state, EntryList *list);
void free_list(EntryList *list);

void subdir_stack_push(AppState *state, Stack *stack, EntryList *list);
void subdir_stack_pop(AppState *state, Stack *stack, EntryList *list);
void free_stack(Stack *stack);

void restore_cursor(AppState *state, EntryList *list);

void navigate_subdir(AppState *state, EntryList *list);
void navigate_root(AppState *state);

void delete_entry(AppState *state, EntryList *list);
void open_entry(AppState *state, EntryList *list);
void create_entry(char *name, AppState *state);

char *helper_set_full_path(char *buffer, size_t buffer_size, char *name, AppState *state);

#endif
