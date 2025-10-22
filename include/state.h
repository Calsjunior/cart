#ifndef STATE_H
#define STATE_H

#include <stdbool.h>

typedef enum
{
    MODE_NORMAL,
    MODE_PROMPT
} AppMode;

typedef enum
{
    PROMPT_NONE,
    PROMPT_HELP,
    PROMPT_DELETE
} PromptType;

typedef struct
{
    char *dir_path;
    char *cursor_name;
    int scroll_offset;
    bool refresh;
    bool running;
    bool restore_cursor;
    AppMode mode;
} AppState;

#endif
