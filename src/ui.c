#define _DEFAULT_SOURCE

#include <ncurses.h>
#include <string.h>

#include "colors.h"
#include "fs.h"
#include "icon.h"
#include "keymap.h"
#include "state.h"
#include "ui.h"

static void draw_file_browser(AppState *state, EntryList *list);
static void draw_path_line(AppState *state);
static void draw_status_line(AppState *state, EntryList *list);
static void draw_keymap_help(void);
static void draw_delete_entry_prompt(EntryList *list);
static void draw_create_entry_prompt(AppState *state);

static void adjust_scroll(int visible_lines, EntryList *list);
static int get_cursor_position(EntryList *list);

static void handle_normal_mode(Action key, AppState *state, Stack *stack, EntryList *list);
static void handle_prompt_mode(Action key, AppState *state, Stack *stack, EntryList *list);

static void helper_set_mode_normal(AppState *state);

static int center_text_menu(int width, const char *text);
static void truncate_middle(char *dest, size_t dest_size, const char *src, int max_width);

static void do_resize(void);

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
                draw_delete_entry_prompt(list);
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

    if (state->mode == MODE_NORMAL)
    {
        handle_normal_mode(key, state, stack, list);
        return;
    }

    if (state->mode == MODE_PROMPT)
    {
        handle_prompt_mode(key, state, stack, list);
        return;
    }
}

void clean_ui()
{
    endwin();
}

static void draw_file_browser(AppState *state, EntryList *list)
{
    clear();
    draw_path_line(state);

    // Calculate visible space (between top path and bottom status)
    int visible_lines = max_rows - 3; // -2 for path bar, -1 for status bar

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
        if (row >= max_rows)
        {
            break;
        }

        if (current == list->cursor)
        {
            attron(A_REVERSE);
        }

        if (current->type == ENTRY_DIR)
        {
            mvprintw(row, 1, " %s%s", get_file_icon(current->name, ENTRY_DIR), current->name);
        }
        else
        {
            mvprintw(row, 1, " %s%s", get_file_icon(current->name, ENTRY_FILE), current->name);
        }

        attroff(A_REVERSE);
        row++;
        entry_index++;
    }

    draw_status_line(state, list);
    refresh();
}

static void draw_path_line(AppState *state)
{
    int path_row = 0;
    mvprintw(path_row, 0, " %s", state->dir_path);
    apply_color(THEME_PATH_LINE);
    for (int i = 0; i < max_cols; i++)
    {
        mvaddch(path_row + 1, i, ACS_HLINE);
    }
    unapply_color(THEME_PATH_LINE);
}

static void draw_status_line(AppState *state, EntryList *list)
{
    int status_row = max_rows - 1;
    int current_x = 0;

    // Print the bar
    apply_color(THEME_STATUS_LINE);
    for (int i = 0; i < max_cols; i++)
    {
        mvaddch(status_row, i, ' ');
    }
    unapply_color(THEME_STATUS_LINE);

    // Left section
    char *mode_str;
    if (state->mode == MODE_NORMAL)
    {
        mode_str = "NORMAL";
    }
    else
    {
        mode_str = "PROMPT";
    }
    apply_color(THEME_STATUS_MODE);
    mvprintw(status_row, current_x, " %-7s", mode_str);
    unapply_color(THEME_STATUS_MODE);

    current_x = strlen(mode_str);

    // Separator
    apply_color(THEME_STATUS_SEPARATOR);
    mvprintw(status_row, current_x + 2, "");
    unapply_color(THEME_STATUS_SEPARATOR);

    if (list->cursor == NULL)
    {
        return;
    }
    int str_width = 30;
    char filename[str_width];
    truncate_middle(filename, sizeof(filename), list->cursor->name, str_width);
    apply_color(THEME_STATUS_INFO);
    mvprintw(status_row, current_x + 3, " %s", filename);
    unapply_color(THEME_STATUS_INFO);

    // Right section
    char right_buffer[128];
    int cursor_position = get_cursor_position(list) + 1;
    snprintf(right_buffer, sizeof(right_buffer), "%4d/%-4d", cursor_position, list->count_entries);
    int right_x = max_cols - strlen(right_buffer);

    apply_color(THEME_STATUS_POSITION);
    mvprintw(status_row, right_x, "%s", right_buffer);
    unapply_color(THEME_STATUS_POSITION);

    apply_color(THEME_STATUS_SEPARATOR);
    mvprintw(status_row, right_x, "");
    unapply_color(THEME_STATUS_SEPARATOR);

    // Display the percentage section
    char pos_percent[10];
    char *pos_top = "Top";
    char *pos_bottom = "Bot";
    float percent = (float) cursor_position / list->count_entries * 100;
    if (list->cursor == list->head)
    {
        snprintf(pos_percent, sizeof(pos_percent), "%s ", pos_top);
    }
    else if (list->cursor == list->tail)
    {
        snprintf(pos_percent, sizeof(pos_percent), "%s ", pos_bottom);
    }
    else
    {
        snprintf(pos_percent, sizeof(pos_percent), "%-2.0f%% ", percent);
    }
    right_x = right_x - strlen(pos_percent);

    apply_color(THEME_STATUS_PERCENT);
    mvprintw(status_row, right_x, "%s", pos_percent);
    unapply_color(THEME_STATUS_PERCENT);
}

// TODO: fix these disguisting UI madness
static void draw_keymap_help(void)
{
    int height, width, start_rows, start_cols;
    height = max_rows / 2;
    width = max_cols / 3;
    start_rows = (max_rows - height) / 2;
    start_cols = (max_cols - width) / 2;
    int left_align_cols = 2;
    WINDOW *key_help = newwin(height, width, start_rows, start_cols);
    refresh();

    const char *title = "Keymaps";
    int title_cols = center_text_menu(width, title);
    mvwprintw(key_help, 1, title_cols, "%s", title);

    const char *nav = "Navigation: ";
    mvwprintw(key_help, 3, left_align_cols, "%s", nav);

    const char *move_keys = " /   or k/j  -  Move up/down";
    const char *nav_keys = " /   or h/l  -  Move left/right";
    int move_keys_cols = center_text_menu(width, move_keys);
    mvwprintw(key_help, 5, move_keys_cols + 1, "%s", move_keys);
    mvwprintw(key_help, 6, move_keys_cols + 1, "%s", nav_keys);

    const char *move_half = "PgDn/PgUp or CTRL D/CTRL U  -  Move half up/down";
    int move_half_cols = center_text_menu(width, move_half);
    mvwprintw(key_help, 7, move_half_cols, "%s", move_half);

    const char *move_all = "Home/End or g/G  -  Go top/bottom";
    int move_all_cols = center_text_menu(width, move_all);
    mvwprintw(key_help, 8, move_all_cols, "%s", move_all);

    const char *action = "Action: ";
    mvwprintw(key_help, 10, left_align_cols, "%s", action);

    const char *create_entry = "a  -  Create file/folder";
    int create_entry_cols = center_text_menu(width, create_entry);
    mvwprintw(key_help, 11, create_entry_cols, "%s", create_entry);

    const char *delete_entry = "d  -  Delete file/folder";
    int delete_entry_cols = center_text_menu(width, delete_entry);
    mvwprintw(key_help, 12, delete_entry_cols, "%s", delete_entry);

    const char *open_entry = "v  -  Open file/folder";
    int open_entry_cols = center_text_menu(width, open_entry);
    mvwprintw(key_help, 13, delete_entry_cols, "%s", open_entry);

    const char *quit = "q - Quit";
    int quit_cols = center_text_menu(width, quit);
    mvwprintw(key_help, height - 2, quit_cols, "%s", quit);

    box(key_help, 0, 0);
    wrefresh(key_help);
}

static void draw_delete_entry_prompt(EntryList *list)
{
    int height, width, start_rows, start_cols;
    height = max_rows / 5;
    width = max_cols / 4;
    start_rows = (max_rows - height) / 4;
    start_cols = (max_cols - width) / 2;
    WINDOW *deletionwin = newwin(height, width, start_rows, start_cols);
    refresh();

    const char *delete_confirmation = "Are you sure you want to delete";
    int delete_confirmation_cols = center_text_menu(width, delete_confirmation);
    mvwprintw(deletionwin, 1, delete_confirmation_cols, "%s", delete_confirmation);

    int selected_cols = center_text_menu(width, list->cursor->name);
    mvwprintw(deletionwin, 2, selected_cols, "%s?", list->cursor->name);

    const char *delete_yes = "1. Yes";
    mvwprintw(deletionwin, 4, 5, "%s", delete_yes);

    mvwprintw(deletionwin, 4, 30, "2. No");

    box(deletionwin, 0, 0);
    wrefresh(deletionwin);
}

static void draw_create_entry_prompt(AppState *state)
{
    int height, width, start_rows, start_cols;
    height = max_rows / 5;
    width = max_cols / 2;
    start_rows = (max_rows - height) / 4;
    start_cols = (max_cols - width) / 2;
    WINDOW *creationwin = newwin(height, width, start_rows, start_cols);
    refresh();

    const char *create_entry = "Add new file to the current working directory (directory ends with '/')";
    int create_entry_cols = center_text_menu(width, create_entry);
    mvwprintw(creationwin, 1, create_entry_cols, "%s", create_entry);

    box(creationwin, 0, 0);

    echo();
    curs_set(TRUE);

    mvwgetnstr(creationwin, 3, 5, state->input, sizeof(state->input) - 1);

    noecho();
    curs_set(FALSE);

    wrefresh(creationwin);
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
            if (list->cursor != NULL && list->cursor->type == ENTRY_DIR)
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
            state->mode = MODE_PROMPT;
            state->prompt_type = PROMPT_DELETE;
            break;

        case OPEN:
            state->mode = MODE_PROMPT;
            state->prompt_type = PROMPT_OPEN;
            break;

        case CREATE:
            state->mode = MODE_PROMPT;
            state->prompt_type = PROMPT_CREATE;
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
            create_entry(state->input, state);
            helper_set_mode_normal(state);
            state->restore_cursor = true;
            state->refresh = true;
            break;
        default:
            break;
    }
}

static void helper_set_mode_normal(AppState *state)
{
    state->mode = MODE_NORMAL;
    state->prompt_type = PROMPT_NONE;
}

static int center_text_menu(int width, const char *text)
{
    return (width - strlen(text)) / 2;
}

static void truncate_middle(char *dest, size_t dest_size, const char *src, int max_width)
{
    int src_len = strlen(src);

    if (src_len <= max_width)
    {
        snprintf(dest, dest_size, "%s", src);
        return;
    }

    int left_len = (max_width - 3) / 2;
    int right_len = max_width - 3 - left_len;

    if (left_len > src_len)
    {
        left_len = src_len;
    }
    if (right_len > src_len)
    {
        right_len = src_len;
    }

    snprintf(dest, dest_size, "%.*s…%.*s", left_len, src, right_len, src + src_len - right_len);
}

static void do_resize(void)
{
    endwin();
    refresh();
    clear();
    getmaxyx(stdscr, max_rows, max_cols);
}
