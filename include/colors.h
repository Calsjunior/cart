#ifndef COLORS_H
#define COLORS_H

#define WCOLOR_ON(win, color) apply_color_win(win, color)
#define WCOLOR_OFF(win, color) unapply_color_win(win, color)

#include <ncurses.h>

typedef enum
{
    COLOR_DEFAULT = -1,
    COLOR_FG_BLACK = 0,
    COLOR_FG_RED,
    COLOR_FG_GREEN,
    COLOR_FG_YELLOW,
    COLOR_FG_BLUE,
    COLOR_FG_MAGENTA,
    COLOR_FG_CYAN,
    COLOR_FG_WHITE,

    COLOR_FG_BRIGHT_BLACK = 8,
    COLOR_FG_BRIGHT_RED,
    COLOR_FG_BRIGHT_GREEN,
    COLOR_FG_BRIGHT_YELLOW,
    COLOR_FG_BRIGHT_BLUE,
    COLOR_FG_BRIGHT_MAGENTA,
    COLOR_FG_BRIGHT_CYAN,
    COLOR_FG_BRIGHT_WHITE,
} ColorID;

typedef enum
{
    /* UI */
    // Status and path line
    THEME_PATH_LINE = 1,
    THEME_STATUS_LINE,
    THEME_STATUS_SEPARATOR,
    THEME_STATUS_MODE,
    THEME_STATUS_INFO,
    THEME_STATUS_POSITION,
    THEME_STATUS_POSITION_SEPARATOR,
    THEME_STATUS_PERCENT,

    // Modal
    THEME_MODAL_BG,
    THEME_MODAL_BORDER,
    THEME_MODAL_TITLE,

    // Deletion prompt
    THEME_DELETE_TITLE,
    THEME_DELETE_OPTION,

    // Keymaps help
    THEME_KEYHELP_SECTION,
    THEME_KEYHELP_KEYS,
    THEME_KEYHELP_DESC,

    /* Browserd */
    // Entry name
    THEME_DIR,
    THEME_FILE,

    // Entry icon
    THEME_DIR_ICON,
    THEME_FILE_ICON,
    THEME_C_ICON,
    THEME_PYTHON_ICON,

    // Selection
    THEME_SELECTED,
} ThemeColor;

typedef struct
{
    ThemeColor theme;
    ColorID fg;
    ColorID bg;
    int attribute;
} ColorPairs;

void init_colors(void);
void apply_color(ThemeColor theme_color);
void unapply_color(ThemeColor theme_color);

void apply_color_win(WINDOW *win, ThemeColor theme_color);
void unapply_color_win(WINDOW *win, ThemeColor theme_color);

#endif
