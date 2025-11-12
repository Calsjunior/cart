#ifndef STATE_H
#define STATE_H

#include <stdbool.h>

#define ENTRY_SIZE 256

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
    PROMPT_CREATE,
    PROMPT_GOTO,
} PromptType;

typedef enum
{
    GOTO_NONE,
    GOTO_PENDING,
} GoToState;

typedef struct
{
    char input[ENTRY_SIZE];
    int input_pos;
    int last_keypress;
} InputState;

typedef struct
{
    char *dir_path;
    char *cursor_name;
    int scroll_offset;
    bool refresh;
    bool running;
    bool restore_cursor;
    bool show_hidden;
    AppMode mode;
    PromptType prompt_type;
    InputState input_state;
    GoToState goto_state;
} AppState;

#endif
