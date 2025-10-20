#ifndef STATE_H
#define STATE_H

#include <stdbool.h>

typedef enum
{
    MODE_NORMAL,
    MODE_PROMPT
} AppMode;

typedef struct
{
    char *dir_path;
    bool refresh;
    bool running;
    AppMode mode;
} AppState;

#endif
