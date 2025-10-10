#include <dirent.h>
#include <ncurses.h>
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
        if (strcmp(dir_entry->d_name, ".") == 0 || strcmp(dir_entry->d_name, "..") == 0)
        {
            continue;
        }
        list->names[list->count] = dir_entry->d_name;
        list->count++;
    }

    closedir(dir);
}
