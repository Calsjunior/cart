#include <dirent.h>
#include <ncurses.h>

#include "fs.h"

void list_dir(const char *dirname)
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
        printw("%s\n", item->d_name);
    }

    closedir(dir);
}
