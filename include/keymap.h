#ifndef KEYMAP_H
#define KEYMAP_H

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
    SELECT,
    DELETE,
    KEYMAP_HELP,
    QUIT
} Action;

Action get_action(int ch);

#endif
