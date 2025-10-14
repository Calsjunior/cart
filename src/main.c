#define _DEFAULT_SOURCE

#include <ncurses.h>
#include <string.h>

#include "ui.h"

int main(void)
{
    init_ui();
    init_keys();

    EntryList list;
    AppState state = {.dir_path = strdup("."), .refresh = true, .running = true};

    int ch;
    while (state.running)
    {
        if (state.refresh)
        {
            clear();
            list_dir(&state, &list);
            state.refresh = false;
        }
        mvprintw(0, 0, "Press up or down arrow key to move, q to quit.\n\n");

        draw_ui(&list);
        ch = getch();

        Action key = get_action(ch);
        handle_input(key, &state, &list);
    }

    clean_ui();
    return 0;
}
