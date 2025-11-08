#include "keymap.h"

Action get_action(int ch, AppState *state)
{
    state->input_state.last_keypress = ch;

    // Universal quit and esc
    switch (ch)
    {
        case 'q':
            if (state->prompt_type != PROMPT_CREATE)
            {
                return QUIT;
            }
            break;
        case 27:
            return ESC;
    }

    if (state->mode == MODE_NORMAL)
    {
        switch (ch)
        {
            case '?':
                return KEYMAP_HELP;
            case KEY_RESIZE:
                return RESIZE;
            case '.':
                return TOGGLE_HIDDEN;

            // Navigation Keys
            case KEY_UP:
            case 'k':
                return MOVE_UP;
            case KEY_DOWN:
            case 'j':
                return MOVE_DOWN;
            case KEY_LEFT:
            case 'h':
                return MOVE_LEFT;
            case KEY_RIGHT:
            case 10:  // Enter key
            case ' ': // Space key
            case 'l':
                return MOVE_RIGHT;
            case KEY_PPAGE:
            case CTRL('u'):
                return MOVE_UP_HALF;
            case KEY_NPAGE:
            case CTRL('d'):
                return MOVE_DOWN_HALF;
            case KEY_HOME:
                return MOVE_UP_ALL;
            case 'g':
                return GOTO;
            case KEY_END:
            case 'G':
                return MOVE_DOWN_ALL;

            // Action
            case KEY_DC:
            case 'd':
                return DELETE;
            case 'v':
                return OPEN;
            case 'a':
                return CREATE;
            default:
                break;
        }
    }

    else if (state->mode == MODE_PROMPT && state->prompt_type == PROMPT_DELETE)
    {
        switch (ch)
        {
            case 'y':
            case 'Y':
            case '1':
                return CONFIRM_YES;
            case 'n':
            case 'N':
            case '2':
                return CONFIRM_NO;
        }
    }

    else if (state->mode == MODE_PROMPT && state->prompt_type == PROMPT_CREATE)
    {
        switch (ch)
        {
            case '\n':
            case '\r':
            case KEY_ENTER:
                return CONFIRM_YES;
            case KEY_BACKSPACE:
            case 128:
                return TEXT_BACKSPACE;
            case KEY_DC:
                return TEXT_DELETE;
            default:
                if (ch >= 32 && ch < 127)
                {
                    return TEXT_INPUT;
                }
                break;
        }
    }
    return -1;
}
