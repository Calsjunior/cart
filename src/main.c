#include <ncurses.h>

#include "fs.h"
#include "input.h"
#include "ui.h"

int main(void)
{
    init_ui();
    init_keys();

    int selected = 0;

    int ch;
    while ((ch = getch()) != 'q')
    {
        mvprintw(0, 0, "Press up or down arrow key to move, q to quit.\n\n");

        list_dir(".");

        Action action = get_action(ch);
        switch (action)
        {
            case MOVE_UP:
                selected--;
                if (selected < 0)
                {
                    selected = 0;
                }
                break;
            case MOVE_DOWN:
                selected++;
                break;
        }
    }

    clean_ui();
    return 0;
}
