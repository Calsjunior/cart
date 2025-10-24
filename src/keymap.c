#include <ncurses.h>

#include "keymap.h"

Action get_action(int ch)
{
    switch (ch)
    {
        case 'q':
            return QUIT;
        case '?':
            return KEYMAP_HELP;
        case KEY_RESIZE:
            return RESIZE;

        // Navigation Keys
        case KEY_UP:
        case 'k':
            return MOVE_UP;
        case KEY_DOWN:
        case 'j':
            return MOVE_DOWN;
        case KEY_LEFT:
        case 'h':
            return MOVE_LEFT;
        case KEY_RIGHT:
        case 10:  // Enter key
        case ' ': // Space key
        case 'l':
            return MOVE_RIGHT;
        case KEY_PPAGE:
        case CTRL('u'):
            return MOVE_UP_HALF;
        case KEY_NPAGE:
        case CTRL('d'):
            return MOVE_DOWN_HALF;
        case KEY_HOME:
        case 'g':
            return MOVE_UP_ALL;
        case KEY_END:
        case 'G':
            return MOVE_DOWN_ALL;

        // Action
        case KEY_DC:
        case 'd':
            return DELETE;
        case 'y':
        case 'Y':
        case '1':
            return CONFIRM_YES;
        case 'n':
        case 'N':
        case '2':
            return CONFIRM_NO;
        case 'v':
            return OPEN;
        default:
            break;
    }
    return -1;
}
