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
    bool restore_cursor;
    AppMode mode;
} AppState;

#endif
