#include "colors.h"

static ColorPairs color_scheme[] = {
    /* UI */
    // Status and path line
    {THEME_PATH_LINE, COLOR_FG_WHITE, COLOR_DEFAULT, 0},
    {THEME_STATUS_LINE, COLOR_DEFAULT, COLOR_FG_BLACK, 0},
    {THEME_STATUS_SEPARATOR, COLOR_FG_GREEN, COLOR_FG_BLACK, 0},
    {THEME_STATUS_MODE, COLOR_FG_BLACK, COLOR_FG_GREEN, A_BOLD},
    {THEME_STATUS_INFO, COLOR_DEFAULT, COLOR_FG_BLACK, A_BOLD},
    {THEME_STATUS_POSITION, COLOR_FG_BLACK, COLOR_FG_GREEN, A_BOLD},
    {THEME_STATUS_PERCENT, COLOR_FG_WHITE, COLOR_BLACK, A_BOLD},

    // Modal
    {THEME_MODAL_BG, COLOR_DEFAULT, COLOR_FG_BLACK, 0},
    {THEME_MODAL_BORDER, COLOR_FG_WHITE, COLOR_FG_BLACK, 0},
    {THEME_MODAL_TITLE, COLOR_FG_WHITE, COLOR_FG_BLACK, A_BOLD},

    // Deletion prompt
    {THEME_DELETE_TITLE, COLOR_FG_RED, COLOR_FG_BLACK, A_BOLD},
    {THEME_DELETE_OPTION, COLOR_FG_WHITE, COLOR_FG_BLACK, A_BOLD},

    // Keymaps help
    {THEME_KEYHELP_KEYS, COLOR_FG_RED, COLOR_FG_BLACK, 0},
    {THEME_KEYHELP_SECTION, COLOR_FG_WHITE, COLOR_FG_BLACK, A_BOLD},
    {THEME_KEYHELP_DESC, COLOR_FG_BLUE, COLOR_FG_BLACK, 0},

    // Browser
    {THEME_DIR, COLOR_FG_BLUE, COLOR_DEFAULT, 0},
    {THEME_FILE, COLOR_DEFAULT, COLOR_DEFAULT, 0},

    // File extension
    {THEME_CODE_C, COLOR_FG_BLUE, COLOR_DEFAULT, 0},

    // Selection
    {THEME_SELECTED, COLOR_DEFAULT, COLOR_DEFAULT, A_REVERSE},
};

static int num_colors = sizeof(color_scheme) / sizeof(color_scheme[0]);

void init_colors(void)
{
    if (!has_colors())
    {
        return;
    }

    start_color();
    use_default_colors();

    for (int i = 0; i < num_colors; i++)
    {
        init_pair(color_scheme[i].theme, color_scheme[i].fg, color_scheme[i].bg);
    }
}

void apply_color(ThemeColor theme_color)
{
    apply_color_win(stdscr, theme_color);
}

void unapply_color(ThemeColor theme_color)
{
    unapply_color_win(stdscr, theme_color);
}

void apply_color_win(WINDOW *win, ThemeColor theme_color)
{
    if (win == NULL)
    {
        win = stdscr;
    }

    for (int i = 0; i < num_colors; i++)
    {
        if (color_scheme[i].theme == theme_color)
        {
            wattron(win, COLOR_PAIR(theme_color) | color_scheme[i].attribute);
            return;
        }
    }
}

void unapply_color_win(WINDOW *win, ThemeColor theme_color)
{
    if (win == NULL)
    {
        win = stdscr;
    }

    for (int i = 0; i < num_colors; i++)
    {
        if (color_scheme[i].theme == theme_color)
        {
            wattroff(win, COLOR_PAIR(theme_color) | color_scheme[i].attribute);
            return;
        }
    }
}
