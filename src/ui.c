#define _DEFAULT_SOURCE

#include <ncurses.h>
#include <string.h>

#include "fs.h"
#include "keymap.h"
#include "state.h"
#include "ui.h"

static int get_cursor_position(EntryList *list);
static void adjust_scroll(int visible_lines, EntryList *list);
static void navigation_input(Action key, AppState *state, Stack *stack, EntryList *list);

int max_rows = 0;
int max_cols = 0;

void init_ui(void)
{
    initscr();            // Initializes screen
    cbreak();             // Disable line buffering (waiting for enter)
    noecho();             // Don't echo keypresses to screen
    curs_set(FALSE);      // Display cursor
    keypad(stdscr, TRUE); // Enable special keys like arrow keys
    halfdelay(TRUE);
    getmaxyx(stdscr, max_rows, max_cols);

    if (has_colors())
    {
        start_color();
        use_default_colors();

        init_pair(1, COLOR_CYAN, -1);
    }
}

void draw_ui(AppState *state, EntryList *list)
{
    clear();

    mvprintw(0, 0, "Path: %s", state->dir_path);

    for (int i = 0; i < max_cols; i++)
    {
        mvaddch(1, i, ACS_HLINE);
    }

    // Calculate visible space (between top path and bottom status)
    int visible_lines = max_rows - 4; // -1 for path, -1 for separator, -2 for status

    adjust_scroll(visible_lines, list);

    int row = 2;
    int entry_index = 0;

    for (EntryNode *current = list->head; current != NULL; current = current->next)
    {
        // Skip entries before scroll offset
        if (entry_index < list->scroll_offset)
        {
            entry_index++;
            continue;
        }

        // Stop drawing if filled the visible area
        if (row >= max_rows - 2)
        {
            break;
        }

        if (current == list->cursor)
        {
            attron(A_REVERSE);
        }

        if (current->type == ENTRY_DIR)
        {
            mvprintw(row, 2, "[DIR]  %s", current->name);
        }
        else
        {
            mvprintw(row, 2, "[FILE] %s", current->name);
        }

        attroff(A_REVERSE);
        row++;
        entry_index++;
    }

    for (int i = 0; i < max_cols; i++)
    {
        mvaddch(max_rows - 2, i, ACS_HLINE);
    }

    int cursor_position = get_cursor_position(list);
    mvprintw(max_rows - 1, 0, "Entries: %d | Position: %d | Keymaps: (?)", list->count_entries, cursor_position + 1);

    refresh();
}

void keymap_help(void)
{
    int height, width, start_rows, start_cols;
    height = 15;
    width = 60;
    start_rows = (max_rows - height) / 2;
    start_cols = (max_cols - width) / 2;
    WINDOW *key_help = newwin(height, width, start_rows, start_cols);
    refresh();

    const char *title = "------- Keymaps -------";
    int cols = (start_cols + strlen(title)) / 4;

    mvwprintw(key_help, 1, cols, title);
    mvwprintw(key_help, 3, 2, "Navigation:");
    mvwprintw(key_help, 4, 2, "↑/↓ or k/j  - Move up/down");
    box(key_help, 0, 0);
    wrefresh(key_help);
}

void handle_input(Action key, AppState *state, Stack *stack, EntryList *list)
{
    if (key == KEYMAP_HELP)
    {
        state->mode = MODE_PROMPT;
        return;
    }

    if (key == QUIT && state->mode == MODE_PROMPT)
    {
        state->mode = MODE_NORMAL;
        return;
    }

    if (state->mode == MODE_NORMAL)
    {
        navigation_input(key, state, stack, list);
    }
}

static int get_cursor_position(EntryList *list)
{
    int position = 0;
    for (EntryNode *current = list->head; current != NULL; current = current->next)
    {
        if (current == list->cursor)
        {
            return position;
        }
        position++;
    }
    return 0;
}

static void adjust_scroll(int visible_lines, EntryList *list)
{
    int cursor_position = get_cursor_position(list);

    // If cursor is above visible area, scroll up
    if (cursor_position < list->scroll_offset)
    {
        list->scroll_offset = cursor_position;
    }

    // If cursor is below visible area, scroll down
    if (cursor_position >= list->scroll_offset + visible_lines)
    {
        list->scroll_offset = cursor_position - visible_lines + 1;
    }
}

static void navigation_input(Action key, AppState *state, Stack *stack, EntryList *list)
{
    if (key == QUIT)
    {
        state->running = false;
    }
    else if (key == MOVE_UP && list->cursor != NULL && list->cursor->prev != NULL)
    {
        list->cursor = list->cursor->prev;
    }
    else if (key == MOVE_UP_HALF)
    {
        for (int i = 0; i < HALF_PAGE; i++)
        {
            if (list->cursor != NULL && list->cursor->prev != NULL)
            {
                list->cursor = list->cursor->prev;
            }
        }
    }
    else if (key == MOVE_UP_ALL)
    {
        list->cursor = list->head;
    }
    else if (key == MOVE_DOWN && list->cursor != NULL && list->cursor->next != NULL)
    {
        list->cursor = list->cursor->next;
    }
    else if (key == MOVE_DOWN_HALF)
    {
        for (int i = 0; i < HALF_PAGE; i++)
        {
            if (list->cursor != NULL && list->cursor->next != NULL)
            {
                list->cursor = list->cursor->next;
            }
        }
    }
    else if (key == MOVE_DOWN_ALL)
    {
        list->cursor = list->tail;
    }
    else if (key == MOVE_RIGHT && list->cursor->type == ENTRY_DIR)
    {
        subdir_stack_push(state, stack, list);
        navigate_subdir(state, list);
        state->refresh = true;
    }
    else if (key == MOVE_LEFT && stack->top != NULL)
    {
        subdir_stack_pop(state, stack, list);
        state->refresh = true;
    }
    else if (key == MOVE_LEFT && strcmp(state->dir_path, "/") != 0)
    {
        navigate_root(state);
        state->refresh = true;
    }
}

void clean_ui()
{
    endwin();
}
