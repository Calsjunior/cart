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
    init_keys();

    const char *home = getenv("HOME");
    EntryList list = {.head = NULL, .tail = NULL, .cursor = NULL, .count_entries = 0, .scroll_offset = 0};
    AppState state = {.dir_path = strdup(home), .refresh = true, .running = true, .mode = MODE_NORMAL};
    Stack stack = {.top = NULL};

    int ch;
    while (state.running)
    {
        if (state.refresh)
        {
            clear();
            list_dir(&state, &list);

            if (stack.top != NULL)
            {
                stack_restore_cursor(&stack, &list);
            }
            state.refresh = false;
        }

        draw_ui(&state, &list);

        if (state.mode == MODE_PROMPT)
        {
            keymap_help();
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
