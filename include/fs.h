#ifndef FS_H
#define FS_H

#include "state.h"

#define PATH_MAX 4096

// Navigating in the current working directory
typedef enum
{
    ENTRY_DIR,
    ENTRY_FILE
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
    struct StackNode *next;
} StackNode;

typedef struct Stack
{
    StackNode *top;
} Stack;

void list_dir(AppState *state, EntryList *list);
void free_list(EntryList *list);

void subdir_stack_push(AppState *state, Stack *stack);
void subdir_stack_pop(AppState *state, Stack *stack, EntryList *list);
void free_stack(Stack *stack);

void save_cursor_state(AppState *state, EntryList *list);
void restore_cursor(AppState *state, EntryList *list);

void navigate_subdir(AppState *state, EntryList *list);
void navigate_root(AppState *state);

void delete_entry(AppState *state, EntryList *list);

#endif
