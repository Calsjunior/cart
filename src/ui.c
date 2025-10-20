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

static int center_text_menu(int width, const char *text);
static int left_align_text_menu(int width, const char *text);
static int right_align_text_menu(int width, const char *text);

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
            mvprintw(row, 2, "   %s", current->name);
        }
        else
        {
            mvprintw(row, 2, "  %s", current->name);
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
    height = max_rows / 2;
    width = max_cols / 3;
    start_rows = (max_rows - height) / 2;
    start_cols = (max_cols - width) / 2;
    WINDOW *key_help = newwin(height, width, start_rows, start_cols);
    refresh();

    const char *title = "Keymaps";
    int title_cols = center_text_menu(width, title);
    mvwprintw(key_help, 1, title_cols, "%s", title);

    const char *nav = "Navigation: ";
    int nav_cols = left_align_text_menu(width, nav);
    mvwprintw(key_help, 3, nav_cols, "%s", nav);

    const char *move_keys = " /   or k/j  -  Move up/down";
    const char *nav_keys = " /   or h/l  -  Move left/right";
    int move_keys_cols = center_text_menu(width, move_keys);
    mvwprintw(key_help, 5, move_keys_cols + 1, "%s", move_keys);
    mvwprintw(key_help, 6, move_keys_cols + 1, "%s", nav_keys);

    const char *move_half = "PgDn/PgUp or CTRL D/CTRL U  -  Move half up/down";
    int move_half_cols = center_text_menu(width, move_half);
    mvwprintw(key_help, 7, move_half_cols, "%s", move_half);

    const char *move_all = "Home/End or G/SHIFT G  -  Go top/bottom";
    int move_all_cols = center_text_menu(width, move_all);
    mvwprintw(key_help, 8, move_all_cols, "%s", move_all);

    const char *quit = "q - Quit";
    int quit_cols = center_text_menu(width, quit);
    mvwprintw(key_help, height - 2, quit_cols, "%s", quit);

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
    else if (key == MOVE_RIGHT && list->cursor != NULL && list->cursor->type == ENTRY_DIR)
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

static int center_text_menu(int width, const char *text)
{
    return (width - strlen(text)) / 2;
}

static int left_align_text_menu(int width, const char *text)
{
    return (width / strlen(text) - 1);
}

static int right_align_text_menu(int width, const char *text)
{
    return width - strlen(text) - 1;
}

void clean_ui()
{
    endwin();
}
