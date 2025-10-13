#ifndef STATE_H
#define STATE_H

#include <stdbool.h>

typedef struct
{
    char *dir_path;
    int cursor_index;
    bool refresh;
    bool running;
} AppState;

#endif
