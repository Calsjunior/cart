#ifndef FS_H
#define FS_H

#include "state.h"

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

void free_list(EntryList *list);
void add_entry_node(char *name, EntryType type, EntryList *list);
void list_dir(AppState *state, EntryList *list);

void free_stack(Stack *stack);
void subdir_stack_push(AppState *state, Stack *stack);
void subdir_stack_pop(AppState *state, Stack *stack);

#endif
