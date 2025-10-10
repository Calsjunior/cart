#ifndef FS_H
#define FS_H

#define MAX_ITEMS 256

typedef struct
{
    char *names[MAX_ITEMS];
    int count;
} DirList;

void list_dir(const char *dirname, DirList *list);

#endif
