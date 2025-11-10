#include "components/file_browser.h"

void draw_file_browser(AppState *state, EntryList *list)
{
    clear();
    draw_path_line(state);

    // Calculate visible space (between top path and bottom status)
    int visible_lines = max_rows - PATH_LINE - STATUS_LINE;

    adjust_scroll(visible_lines, list);

    int row = SCROLL_PADDING;
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

        const char *icon = get_entry_icon(current->name, current->type);
        ThemeColor icon_color = get_entry_icon_color(current->name, current->type);
        ThemeColor entry_color = get_entry_color(current->name, current->type);

        // Apply icon colors except the cursor, so the selection can match the entry's color
        if (current == list->cursor)
        {
            WCOLOR_ON(stdscr, THEME_SELECTED);
            WCOLOR_ON(stdscr, entry_color);
            mvprintw(row, 1, " %s", icon);
            WCOLOR_OFF(stdscr, entry_color);
        }
        else
        {
            WCOLOR_ON(stdscr, icon_color);
            mvprintw(row, 1, " %s", icon);
            WCOLOR_OFF(stdscr, icon_color);
        }

        if (current->type == ENTRY_SYMLINK_DIR || current->type == ENTRY_SYMLINK_FILE)
        {
            char full_path[PATH_MAX];
            helper_set_full_path(full_path, sizeof(full_path), state->dir_path, current->name);

            char target[PATH_MAX];
            ssize_t len = readlink(full_path, target, sizeof(target) - 1);
            if (len == -1)
            {
                return;
            }
            target[len] = '\0';
            mvprintw(row, 3, " %s -> %s", current->name, target);
        }
        else
        {
            WCOLOR_ON(stdscr, entry_color);
            mvprintw(row, 3, " %s", current->name);
            WCOLOR_OFF(stdscr, entry_color);
        }

        WCOLOR_OFF(stdscr, THEME_SELECTED);
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
    WCOLOR_ON(stdscr, THEME_PATH_LINE);
    for (int i = 0; i < max_cols; i++)
    {
        mvaddch(path_row + 1, i, ACS_HLINE);
    }
    WCOLOR_OFF(stdscr, THEME_PATH_LINE);
}

static void draw_status_line(AppState *state, EntryList *list)
{
    int status_row = max_rows - 1;
    int current_x = 0;

    // Print the bar
    WCOLOR_ON(stdscr, THEME_STATUS_LINE);
    for (int i = 0; i < max_cols; i++)
    {
        mvaddch(status_row, i, ' ');
    }
    WCOLOR_OFF(stdscr, THEME_STATUS_LINE);

    /* Left section */
    char *mode_str;
    if (state->mode == MODE_NORMAL)
    {
        mode_str = "NORMAL";
    }
    else
    {
        mode_str = "PROMPT";
    }
    WCOLOR_ON(stdscr, THEME_STATUS_MODE);
    mvprintw(status_row, current_x, " %-7s", mode_str);
    WCOLOR_OFF(stdscr, THEME_STATUS_MODE);

    current_x = strlen(mode_str);

    // Separator
    WCOLOR_ON(stdscr, THEME_STATUS_SEPARATOR);
    mvprintw(status_row, current_x + 2, "");
    WCOLOR_OFF(stdscr, THEME_STATUS_SEPARATOR);

    // Filename
    if (list->cursor != NULL)
    {
        int str_width = 30;
        char filename[str_width];
        truncate_middle(filename, sizeof(filename), list->cursor->name, str_width);
        WCOLOR_ON(stdscr, THEME_STATUS_INFO);
        mvprintw(status_row, current_x + 3, " %s", filename);
        WCOLOR_OFF(stdscr, THEME_STATUS_INFO);
    }

    /* Right section */
    char right_buffer[128];
    int cursor_position = get_cursor_position(list);
    if (list->cursor != NULL)
    {
        cursor_position++;
    }
    snprintf(right_buffer, sizeof(right_buffer), "%4d/%-4d", cursor_position, list->count_entries);
    int right_x = max_cols - strlen(right_buffer);

    WCOLOR_ON(stdscr, THEME_STATUS_POSITION);
    mvprintw(status_row, right_x, "%s", right_buffer);
    WCOLOR_OFF(stdscr, THEME_STATUS_POSITION);

    WCOLOR_ON(stdscr, THEME_STATUS_SEPARATOR);
    mvprintw(status_row, right_x, "");
    WCOLOR_OFF(stdscr, THEME_STATUS_SEPARATOR);

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

    WCOLOR_ON(stdscr, THEME_STATUS_PERCENT);
    mvprintw(status_row, right_x, "%s", pos_percent);
    WCOLOR_OFF(stdscr, THEME_STATUS_PERCENT);
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
