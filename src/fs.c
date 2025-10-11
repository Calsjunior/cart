#define _DEFAULT_SOURCE

#include <dirent.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "fs.h"

void list_dir(const char *dirname, DirList *list)
{
    DIR *dir = opendir(dirname);
    if (dir == NULL)
    {
        clear();
        mvprintw(0, 0, "Directory cannot be openned.");
        return;
    }

    struct dirent *dir_entry;
    while ((dir_entry = readdir(dir)) != NULL)
    {
        // Incase the directory entry has more than the magic 256
        if (list->count >= MAX_ITEMS)
        {
            break;
        }

        if (strcmp(dir_entry->d_name, ".") == 0 || strcmp(dir_entry->d_name, "..") == 0)
        {
            continue;
        }

        if (dir_entry->d_type == DT_DIR)
        {
            list->entries[list->count].type = ENTRY_DIR;
        }
        else if (dir_entry->d_type == DT_REG)
        {
            list->entries[list->count].type = ENTRY_FILE;
        }
        list->entries[list->count].name = dir_entry->d_name;
        list->count++;

        qsort(list->entries, list->count, sizeof(DirEntry), compare);
    }

    closedir(dir);
}

int compare(const void *arg1, const void *arg2)
{
    const DirEntry *direntryA = arg1;
    const DirEntry *direntryB = arg2;

    // Sort directories first then files
    if (direntryA->type != direntryB->type)
    {
        return (direntryA->type - direntryB->type);
    }

    return strcmp(direntryA->name, direntryB->name);
}
