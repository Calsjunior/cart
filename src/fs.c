#include <dirent.h>
#include <ncurses.h>

#include "fs.h"

void list_dir(const char *dirname, FileEntry *entry)
{
    DIR *dir = opendir(dirname);
    if (dir == NULL)
    {
        clear();
        mvprintw(0, 0, "Directory cannot be openned.");
        return;
    }

    struct dirent *item;
    while ((item = readdir(dir)) != NULL)
    {
        entry->item_entry[entry->item_count] = item->d_name;
        entry->item_count++;
    }

    closedir(dir);
}
