#include <ncurses.h>

int main(void)
{
    initscr();            // Initializes screen
    noecho();             // Don't echo keypresses to screen
    curs_set(FALSE);      // Hide cursor
    keypad(stdscr, TRUE); // Enable special keys like arrow keys

    const char *files[] = {"main.c", "ui.c", "fs.c", "README.md", "LICENSE"};
    int number_of_files = sizeof(files) / sizeof(files[0]);

    int ch;
    while ((ch = getch()) != 'q')
    {
        clear();

        for (int i = 0; i < number_of_files; i++)
        {
            printw("%s\n", files[i]);
        }
        // mvprintw(0, 0, "Press q to quit.");
        // mvprintw(2, 0, "You pressed: %c", ch);

        refresh();
    }

    endwin();
    return 0;
}
