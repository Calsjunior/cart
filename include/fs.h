#ifndef FS_H
#define FS_H

#include "state.h"

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

void add_entry_node(char *name, EntryType type, EntryList *list);
void list_dir(AppState *state, EntryList *list);

#endif
