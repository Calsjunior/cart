#include <ncurses.h>

#include "keymap.h"

KeyMapping keys[MAX_KEYS];

void init_keys(void)
{
    keys[MOVE_UP] = (KeyMapping) {KEY_UP, 'k'};
    keys[MOVE_DOWN] = (KeyMapping) {KEY_DOWN, 'j'};
    keys[MOVE_LEFT] = (KeyMapping) {KEY_LEFT, 'h'};
    keys[MOVE_RIGHT] = (KeyMapping) {KEY_RIGHT, 'l'};
    keys[QUIT] = (KeyMapping) {'q', 'q'};
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
