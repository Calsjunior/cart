#ifndef FS_H
#define FS_H

#define MAX_ITEMS 256

typedef struct
{
    char *item_entry[MAX_ITEMS];
    int item_count;
} FileEntry;

void list_dir(const char *dirname, FileEntry *entry);

#endif
