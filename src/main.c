#include <ncurses.h>

#include "fs.h"
#include "input.h"
#include "ui.h"

int main(void)
{
    init_ui();
    init_keys();

    DirList list = {0};
    list_dir(".", &list);

    int cursor_index = 0;
    int ch;
    while ((ch = getch()) != 'q')
    {
        mvprintw(0, 0, "Press up or down arrow key to move, q to quit.\n\n");

        for (int i = 0; i < list.count; i++)
        {
            if (cursor_index == i)
            {
                attron(A_REVERSE);
            }
            if (list.entries[i].type == ENTRY_DIR)
            {
                mvprintw(i + 2, 2, "%s/", list.entries[i].name);
            }
            else if (list.entries[i].type == ENTRY_FILE)
            {
                mvprintw(i + 2, 2, "%s", list.entries[i].name);
            }
            attroff(A_REVERSE);
        }

        Action action = get_action(ch);
        if (action == MOVE_UP && cursor_index > 0)
        {
            cursor_index--;
        }
        else if (action == MOVE_DOWN && cursor_index < list.count - 1)
        {
            cursor_index++;
        }
    }

    clean_ui();
    return 0;
}
