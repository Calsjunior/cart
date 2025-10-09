#include <ncurses.h>

#include "fs.h"
#include "input.h"

int main(void)
{
    int selected = 0;

    initscr();            // Initializes screen
    cbreak();             // Disable line buffering (waiting for enter)
    noecho();             // Don't echo keypresses to screen
    curs_set(FALSE);      // Display cursor
    keypad(stdscr, TRUE); // Enable special keys like arrow keys
    nodelay(stdscr, TRUE);

    init_keys();

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

    endwin();
    return 0;
}
