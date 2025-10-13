#include <ncurses.h>

#include "ui.h"

int main(void)
{
    init_ui();
    init_keys();

    DirList list;
    AppState state = {.dir_path = "..", .refresh = true, .running = true};

    int ch;
    while (state.running)
    {
        if (state.refresh)
        {
            clear();
            list_dir(state.dir_path, &list);
            state.refresh = false;
        }
        mvprintw(0, 0, "Press up or down arrow key to move, q to quit.\n\n");

        ch = getch();
        draw_ui(&list, &state);

        Action key = get_action(ch);
        handle_input(key, &list, &state);
    }

    clean_ui();
    return 0;
}
