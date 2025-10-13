#include <ncurses.h>

#include "fs.h"
#include "keymap.h"
#include "state.h"
#include "ui.h"

void init_ui(void)
{
    initscr();            // Initializes screen
    cbreak();             // Disable line buffering (waiting for enter)
    noecho();             // Don't echo keypresses to screen
    curs_set(FALSE);      // Display cursor
    keypad(stdscr, TRUE); // Enable special keys like arrow keys
    nodelay(stdscr, TRUE);
}

void draw_ui(DirList *list, AppState *state)
{
    for (int i = 0; i < list->count; i++)
    {
        if (state->cursor_index == i)
        {
            attron(A_REVERSE);
        }
        if (list->entries[i].type == ENTRY_DIR)
        {
            mvprintw(i + 2, 2, "%s/", list->entries[i].name);
        }
        else if (list->entries[i].type == ENTRY_FILE)
        {
            mvprintw(i + 2, 2, "%s", list->entries[i].name);
        }
        attroff(A_REVERSE);
    }
}

void handle_input(Action key, DirList *list, AppState *state)
{
    if (key == QUIT)
    {
        state->running = false;
        return;
    }
    if (key == MOVE_UP && state->cursor_index > 0)
    {
        state->cursor_index--;
    }
    else if (key == MOVE_DOWN && state->cursor_index < list->count - 1)
    {
        state->cursor_index++;
    }
    else if (key == MOVE_RIGHT && list->entries[state->cursor_index].type == ENTRY_DIR)
    {
        if (state->dir_path != list->entries[state->cursor_index].name)
        {
            state->dir_path = list->entries[state->cursor_index].name;
            state->refresh = true;
        }
    }
}

void clean_ui()
{
    endwin();
}
