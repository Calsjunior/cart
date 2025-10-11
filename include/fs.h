#ifndef FS_H
#define FS_H

#define MAX_ITEMS 256

typedef enum
{
    ENTRY_DIR,
    ENTRY_FILE
} EntryType;

typedef struct
{
    char *name;
    EntryType type;
} DirEntry;

typedef struct
{
    DirEntry entries[MAX_ITEMS];
    int count;
} DirList;

void list_dir(const char *dirname, DirList *list);
int compare(const void *arg1, const void *arg2);

#endif
