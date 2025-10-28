#ifndef COLORS_H
#define COLORS_H

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
    // UI
    THEME_STATUS_BAR,
    THEME_STATUS_MODE,
    THEME_STATUS_INFO,
    THEME_STATUS_POSITION,
    THEME_BORDER,
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

#endif
