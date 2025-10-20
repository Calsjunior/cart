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

void free_list(EntryList *list);
void list_dir(AppState *state, EntryList *list);

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

void free_stack(Stack *stack);
void subdir_stack_push(AppState *state, Stack *stack, EntryList *list);
void subdir_stack_pop(AppState *state, Stack *stack, EntryList *list);
void stack_restore_cursor(Stack *stack, EntryList *list);

void navigate_subdir(AppState *state, EntryList *list);
void navigate_root(AppState *state);

#endif
