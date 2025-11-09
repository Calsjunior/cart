#include "ui.h"

static void handle_normal_mode(Action key, AppState *state, Stack *stack, EntryList *list);
static void handle_prompt_mode(Action key, AppState *state, Stack *stack, EntryList *list);
static void handle_text_input(Action key, AppState *state);
static void handle_go_to_input(Action key, AppState *state, EntryList *list);

static void helper_set_mode_normal(AppState *state);

static void do_resize(void);

int max_rows = 0;
int max_cols = 0;

void init_ui(void)
{
    initscr();            // Initializes screen
    cbreak();             // Disable line buffering (waiting for enter)
    noecho();             // Don't echo keypresses to screen
    curs_set(0);          // Display cursor
    keypad(stdscr, TRUE); // Enable special keys like arrow keys
    // halfdelay(1);
    set_escdelay(1);
    getmaxyx(stdscr, max_rows, max_cols);
    init_colors(); // From "colors.h"
}

void draw_ui(AppState *state, EntryList *list)
{
    draw_file_browser(state, list);

    if (state->mode == MODE_PROMPT)
    {
        switch (state->prompt_type)
        {
            case PROMPT_HELP:
                draw_keymap_help();
                break;

            case PROMPT_DELETE:
                draw_delete_entry_prompt(state, list);
                break;

            case PROMPT_CREATE:
                draw_create_entry_prompt(state);
                break;

            default:
                break;
        }
    }
}

void handle_input(Action key, AppState *state, Stack *stack, EntryList *list)
{
    if (key == RESIZE)
    {
        do_resize();
    }

    if (key == QUIT)
    {
        if (state->mode == MODE_PROMPT)
        {
            helper_set_mode_normal(state);
            return;
        }

        // Quit program if mode normal
        state->running = false;
        return;
    }

    if (key == ESC)
    {
        if (state->mode == MODE_PROMPT)
        {
            helper_set_mode_normal(state);
            return;
        }
    }

    if (state->mode == MODE_NORMAL)
    {
        handle_normal_mode(key, state, stack, list);
        return;
    }

    if (state->mode == MODE_PROMPT)
    {
        if (state->prompt_type == PROMPT_CREATE)
        {
            handle_text_input(key, state);
        }
        handle_prompt_mode(key, state, stack, list);
        return;
    }
}

void clean_ui()
{
    endwin();
}

static void handle_normal_mode(Action key, AppState *state, Stack *stack, EntryList *list)
{
    switch (key)
    {
        case MOVE_UP:
            if (list->cursor != NULL && list->cursor->prev != NULL)
            {
                list->cursor = list->cursor->prev;
                return;
            }
            if (list->cursor != NULL && list->cursor->prev == NULL)
            {
                list->cursor = list->tail;
            }
            break;

        case MOVE_DOWN:
            if (list->cursor != NULL && list->cursor->next != NULL)
            {
                list->cursor = list->cursor->next;
                return;
            }
            if (list->cursor != NULL && list->cursor->next == NULL)
            {
                list->cursor = list->head;
            }
            break;

        case MOVE_LEFT:
            if (stack->top != NULL)
            {
                subdir_stack_pop(state, stack, list);
                state->restore_cursor = true;
                state->refresh = true;
                return;
            }

            if (strcmp(state->dir_path, "/") != 0)
            {
                navigate_root(state);
                state->refresh = true;
            }
            break;

        case MOVE_RIGHT:
            if (list->cursor != NULL && (list->cursor->type == ENTRY_DIR || list->cursor->type == ENTRY_SYMLINK_DIR))
            {
                subdir_stack_push(state, stack, list);
                navigate_subdir(state, list);
                state->refresh = true;
            }
            break;

        case MOVE_UP_HALF:
            for (int i = 0; i < max_rows / 2; i++)
            {
                if (list->cursor != NULL && list->cursor->prev != NULL)
                {
                    list->cursor = list->cursor->prev;
                }
            }
            break;

        case MOVE_DOWN_HALF:
            for (int i = 0; i < max_rows / 2; i++)
            {
                if (list->cursor != NULL && list->cursor->next != NULL)
                {
                    list->cursor = list->cursor->next;
                }
            }
            break;

        case MOVE_UP_ALL:
            list->cursor = list->head;
            break;

        case MOVE_DOWN_ALL:
            list->cursor = list->tail;
            break;

        case KEYMAP_HELP:
            state->mode = MODE_PROMPT;
            state->prompt_type = PROMPT_HELP;
            break;

        case DELETE:
            if (list->cursor != NULL)
            {
                state->mode = MODE_PROMPT;
                state->prompt_type = PROMPT_DELETE;
            }
            break;

        case OPEN:
            state->mode = MODE_PROMPT;
            state->prompt_type = PROMPT_OPEN;
            break;

        case CREATE:
            state->mode = MODE_PROMPT;
            state->prompt_type = PROMPT_CREATE;
            break;

        case GOTO:
            state->mode = MODE_PROMPT;
            state->prompt_type = PROMPT_GOTO;
            break;

        case TOGGLE_HIDDEN:
            if (state->show_hidden)
            {
                state->show_hidden = false;
            }
            else
            {
                state->show_hidden = true;
            }

            if (state->cursor_name != NULL)
            {
                free(state->cursor_name);
            }
            state->cursor_name = strdup(list->cursor->name);
            state->restore_cursor = true;
            state->refresh = true;
    }
}

static void handle_prompt_mode(Action key, AppState *state, Stack *stack, EntryList *list)
{
    switch (state->prompt_type)
    {
        case PROMPT_HELP:
            break;

        case PROMPT_DELETE:
            if (key == CONFIRM_YES)
            {
                delete_entry(state, list);
                helper_set_mode_normal(state);
                state->restore_cursor = true;
                state->refresh = true;
                return;
            }

            if (key == CONFIRM_NO)
            {
                helper_set_mode_normal(state);
            }
            break;

        case PROMPT_OPEN:
            open_entry(state, list);
            helper_set_mode_normal(state);
            state->restore_cursor = true;
            state->refresh = true;

        case PROMPT_CREATE:
            if (key == CONFIRM_YES && state->input_state.input_pos > 0)
            {
                create_entry(state->input_state.input, state);
                helper_set_mode_normal(state);
                state->restore_cursor = true;
                state->refresh = true;
            }
            break;

        case PROMPT_GOTO:
            handle_go_to_input(key, state, list);
            break;

        default:
            break;
    }
}

static void handle_text_input(Action key, AppState *state)
{
    int ch = state->input_state.last_keypress;
    switch (key)
    {
        case TEXT_INPUT:
            if (state->input_state.input_pos < ENTRY_SIZE - 1)
            {
                state->input_state.input[state->input_state.input_pos] = (char) ch;
                state->input_state.input_pos++;
                state->input_state.input[state->input_state.input_pos] = '\0';
            }
            break;

        case TEXT_BACKSPACE:
            if (state->input_state.input_pos > 0)
            {
                state->input_state.input_pos--;
                state->input_state.input[state->input_state.input_pos] = '\0';
            }
            break;

        // TODO: Properly implement delete key to delete backwards
        case TEXT_DELETE:
            if (state->input_state.input_pos > 0)
            {
                state->input_state.input_pos--;
                state->input_state.input[state->input_state.input_pos] = '\0';
            }
            break;
    }
}

static void handle_go_to_input(Action key, AppState *state, EntryList *list)
{
    int ch = state->input_state.last_keypress;

    switch (state->goto_state)
    {
        case GOTO_NONE:
            if (key == GOTO_TOP)
            {
                list->cursor = list->head;
                helper_set_mode_normal(state);
                return;
            }

            if (key == GOTO_FIND)
            {
                state->goto_state = GOTO_PENDING;
                return;
            }
            helper_set_mode_normal(state);
            break;

        case GOTO_PENDING:
            if (key == GOTO_CHAR)
            {
                go_to_entry(ch, list);
                state->goto_state = GOTO_NONE;
                helper_set_mode_normal(state);
            }
            break;
    }
}

static void helper_set_mode_normal(AppState *state)
{
    state->mode = MODE_NORMAL;
    state->prompt_type = PROMPT_NONE;
    state->input_state.input[0] = '\0';
    state->input_state.input_pos = 0;
}

static void do_resize(void)
{
    endwin();
    refresh();
    clear();
    getmaxyx(stdscr, max_rows, max_cols);
}
