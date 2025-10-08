#include <ncurses.h>

// Keyboard input
typedef enum
{
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
    MAX_KEYS
} Action;

typedef struct
{
    int arrow;
    int vim;
} KeyMapping;

KeyMapping keys[MAX_KEYS];

Action get_action(int ch);

int main(void)
{
    keys[MOVE_UP] = (KeyMapping) {KEY_UP, 'k'};
    keys[MOVE_DOWN] = (KeyMapping) {KEY_DOWN, 'j'};
    keys[MOVE_LEFT] = (KeyMapping) {KEY_LEFT, 'h'};
    keys[MOVE_RIGHT] = (KeyMapping) {KEY_RIGHT, 'l'};

    const char *files[] = {"main.c", "ui.c", "fs.c", "README.md", "LICENSE"};
    int number_of_files = sizeof(files) / sizeof(files[0]);
    int selected = 0;

    initscr();            // Initializes screen
    cbreak();             // Disable line buffering (waiting for enter)
    noecho();             // Don't echo keypresses to screen
    curs_set(FALSE);      // Display cursor
    keypad(stdscr, TRUE); // Enable special keys like arrow keys
    halfdelay(TRUE);

    int ch;
    while ((ch = getch()) != 'q')
    {
        clear();

        mvprintw(0, 0, "Press up or down arrow key to move, q to quit.\n\n");

        for (int i = 0; i < number_of_files; i++)
        {
            if (i == selected)
            {
                attron(A_REVERSE);
                mvprintw(i + 2, 2, "%s", files[i]);
                attroff(A_REVERSE);
            }
            else
            {
                mvprintw(i + 2, 2, "%s", files[i]);
            }
        }

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
                if (selected >= number_of_files)
                {
                    selected = number_of_files - 1;
                }
                break;
        }
    }

    endwin();
    return 0;
}

Action get_action(int ch)
{
    for (int i = 0; i < MAX_KEYS; i++)
    {
        if (ch == keys[i].arrow || ch == keys[i].vim)
        {
            return i;
        }
    }
    return -1;
}
