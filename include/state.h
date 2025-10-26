#ifndef STATE_H
#define STATE_H

#include <stdbool.h>

#define ENTRY_NAME 256

typedef enum
{
    MODE_NORMAL,
    MODE_PROMPT
} AppMode;

typedef enum
{
    PROMPT_NONE,
    PROMPT_HELP,
    PROMPT_DELETE,
    PROMPT_OPEN,
    PROMPT_CREATE,
} PromptType;

typedef struct
{
    char *dir_path;
    char *cursor_name;
    char input[ENTRY_NAME];
    int scroll_offset;
    bool refresh;
    bool running;
    bool restore_cursor;
    bool show_hidden;
    AppMode mode;
    PromptType prompt_type;
} AppState;

#endif
