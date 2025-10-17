#include "fs.h"
#define _DEFAULT_SOURCE

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "ui.h"

int main(void)
{
    init_ui();
    init_keys();

    EntryList list;
    AppState state = {.dir_path = strdup(".."), .refresh = true, .running = true};
    Stack stack = {.top = NULL};

    int ch;
    while (state.running)
    {
        if (state.refresh)
        {
            clear();
            list_dir(&state, &list);
            state.refresh = false;
        }
        mvprintw(0, 0, "%s", state.dir_path);

        draw_ui(&list);
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
