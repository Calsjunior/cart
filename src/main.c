#include <ncurses.h>

#include "fs.h"
#include "input.h"
#include "ui.h"

int main(void)
{
    init_ui();
    init_keys();

    FileEntry entry = {0};
    list_dir(".", &entry);

    int cursor_index = 0;
    int ch;
    while ((ch = getch()) != 'q')
    {
        mvprintw(0, 0, "Press up or down arrow key to move, q to quit.\n\n");

        for (int i = 0; i < entry.item_count; i++)
        {
            if (cursor_index == i)
            {
                attron(A_REVERSE);
            }
            mvprintw(i + 2, 2, "%s", entry.item_entry[i]);
            attroff(A_REVERSE);
        }

        Action action = get_action(ch);
        switch (action)
        {
            case MOVE_UP:
                if (cursor_index > 0)
                {
                    cursor_index--;
                }
                break;
            case MOVE_DOWN:
                if (cursor_index < entry.item_count - 1)
                {
                    cursor_index++;
                }
                break;
        }
        mvprintw(15, 0, "%i", cursor_index);
        mvprintw(16, 0, "%i", entry.item_count);
    }

    clean_ui();
    return 0;
}
