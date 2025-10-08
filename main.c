#include <ncurses.h>

int main(void)
{
    // Initializes screen
    initscr();

    // Disable line buffering
    raw();

    // Enables special keys
    keypad(stdscr, TRUE);

    // Stops keystrokes from being printed to window
    noecho();

    // Get input
    char ch = getch();
    printw("%c key is pressed!", ch);

    // Refresh screen to match binary
    refresh();
    getch();

    // Deallocates memory and closes screen
    endwin();

    return 0;
}
