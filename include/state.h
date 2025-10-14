#ifndef STATE_H
#define STATE_H

#include <stdbool.h>

typedef struct
{
    char *dir_path;
    bool refresh;
    bool running;
} AppState;

#endif
