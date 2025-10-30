#ifndef KEYMAP_H
#define KEYMAP_H

#include <ncurses.h>

#define CTRL(x) ((x) & 0x1f)

typedef enum
{
    MOVE_UP,
    MOVE_UP_HALF,
    MOVE_UP_ALL,
    MOVE_DOWN,
    MOVE_DOWN_HALF,
    MOVE_DOWN_ALL,
    MOVE_LEFT,
    MOVE_RIGHT,
    DELETE,
    CONFIRM_YES,
    CONFIRM_NO,
    OPEN,
    CREATE,
    RESIZE,
    TOGGLE_HIDDEN,
    KEYMAP_HELP,
    QUIT
} Action;

Action get_action(int ch);

#endif
