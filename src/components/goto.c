#include "components/goto.h"

static GotoHelp goto_mappings[] = {
    {"gg", "Go to top"},
    {"gh", "Go to home dir"},
    {"gf<letter>", "Go to entry name"},
};

static int num_goto = sizeof(goto_mappings) / sizeof(goto_mappings[0]);

void draw_goto_help(void)
{
    ModalConfig config = {
        .title = "Go to",
        .min_height = 10,
        .min_width = 35,
        .height_ratio = 0.3,
        .width_ratio = 0.1,
        .alignment = MODAL_BOTTOM_RIGHT,
        .bg_color = THEME_MODAL_BG,
        .border_color = THEME_MODAL_BORDER,
        .title_color = THEME_MODAL_TITLE,
    };

    int height, width;
    WINDOW *goto_win = create_modal(config, &height, &width);

    int current_row = 2;
    int key_col = 2;
    int desc_col = 15;

    for (int i = 0; i < num_goto; i++)
    {
        if (current_row >= height - 3)
        {
            break;
        }

        GotoHelp mapping = goto_mappings[i];
        WCOLOR_ON(goto_win, THEME_KEYHELP_KEYS);
        mvwprintw(goto_win, current_row, key_col, "%s", mapping.keys);
        WCOLOR_OFF(goto_win, THEME_KEYHELP_KEYS);
        WCOLOR_ON(goto_win, THEME_KEYHELP_DESC);
        mvwprintw(goto_win, current_row, desc_col, "%s", mapping.description);
        WCOLOR_OFF(goto_win, THEME_KEYHELP_DESC);
        current_row++;
    }

    wrefresh(goto_win);
    destroy_modal(goto_win);
}
