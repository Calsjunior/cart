#define _DEFAULT_SOURCE

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

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

void draw_ui(EntryList *list)
{
    int row = 0;
    int col = 0;
    for (EntryNode *current = list->head; current != NULL; current = current->next)
    {
        if (current == list->cursor)
        {
            attron(A_REVERSE);
        }
        if (current->type == ENTRY_DIR)
        {
            mvprintw(row + 2, col, "%s/", current->name);
        }
        else
        {
            mvprintw(row + 2, col, "%s", current->name);
        }
        attroff(A_REVERSE);
        row++;
    }
}

void handle_input(Action key, AppState *state, Stack *stack, EntryList *list)
{
    if (key == QUIT)
    {
        state->running = false;
        return;
    }
    else if (key == MOVE_UP && list->cursor != NULL && list->cursor->prev != NULL)
    {
        list->cursor = list->cursor->prev;
    }
    else if (key == MOVE_DOWN && list->cursor != NULL && list->cursor->next != NULL)
    {
        list->cursor = list->cursor->next;
    }
    else if (key == MOVE_RIGHT && list->cursor->type == ENTRY_DIR)
    {
        char new_path[PATH_MAX];
        snprintf(new_path, sizeof(new_path), "%s/%s", state->dir_path, list->cursor->name);

        free(state->dir_path);
        state->dir_path = strdup(new_path);

        subdir_stack_push(state, stack);
        state->refresh = true;
    }
    else if (key == MOVE_LEFT && stack->top != NULL)
    {
        subdir_stack_pop(state, stack);
        state->refresh = true;
    }
}

void clean_ui()
{
    endwin();
}
