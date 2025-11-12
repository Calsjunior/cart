#ifndef KEYMAP_H
#define KEYMAP_H

#include <ncurses.h>

#include "state.h"

#define CTRL(x) ((x) & 0x1f)

typedef enum
{
    // NORMAL MODE
    MOVE_UP,
    MOVE_UP_HALF,
    MOVE_UP_ALL,
    MOVE_DOWN,
    MOVE_DOWN_HALF,
    MOVE_DOWN_ALL,
    MOVE_LEFT,
    MOVE_RIGHT,
    DELETE,
    GOTO,

    CONFIRM_YES,
    CONFIRM_NO,
    OPEN,
    CREATE,

    // CREATE MODE
    TEXT_INPUT,
    TEXT_BACKSPACE,
    TEXT_DELETE,

    // GOTO MODE
    GOTO_TOP,
    GOTO_HOME,
    GOTO_FIND,
    GOTO_CHAR,

    // General
    RESIZE,
    TOGGLE_HIDDEN,
    KEYMAP_HELP,
    ESC,
    QUIT,
    ACTION_NONE = -1,
} Action;

Action get_action(int ch, AppState *state);

#endif
