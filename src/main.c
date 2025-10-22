#define _DEFAULT_SOURCE

#include <dirent.h>
#include <locale.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "fs.h"
#include "keymap.h"
#include "state.h"
#include "ui.h"

int main(void)
{
    setlocale(LC_ALL, "");
    init_ui();

    const char *home = getenv("HOME");
    EntryList list = {.head = NULL, .tail = NULL, .cursor = NULL, .count_entries = 0, .scroll_offset = 0};
    AppState state = {.dir_path = strdup(home),
                      .cursor_name = NULL,
                      .scroll_offset = 0,
                      .refresh = true,
                      .running = true,
                      .restore_cursor = false,
                      .mode = MODE_NORMAL};
    Stack stack = {.top = NULL};

    int ch;
    while (state.running)
    {
        if (state.refresh)
        {
            clear();
            list_dir(&state, &list);

            if (state.restore_cursor)
            {
                restore_cursor(&state, &list);
                state.restore_cursor = false;
            }
            state.refresh = false;
        }

        draw_ui(&state, &list);

        if (state.mode == MODE_PROMPT)
        {
            draw_keymap_help();
        }
        refresh();
        ch = getch();

        Action key = get_action(ch);
        handle_input(key, &state, &stack, &list);
    }

    free(state.dir_path);
    free_list(&list);
    free_stack(&stack);
    clean_ui();
    return 0;
}
