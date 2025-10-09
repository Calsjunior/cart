#ifndef INPUT_H
#define INPUT_H

typedef enum
{
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
    QUIT,
    MAX_KEYS
} Action;

typedef struct
{
    int arrow;
    int vim;
} KeyMapping;

void init_keys(void);
Action get_action(int ch);

#endif
