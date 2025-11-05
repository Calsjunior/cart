#ifndef MODAL_H
#define MODAL_H

#include <ncurses.h>
#include <stdbool.h>
#include <string.h>

#include "colors.h"

typedef enum
{
    MODAL_CENTER,
    MODAL_TOPHALF_CENTER,
    MODAL_BOTTOM_RIGHT,
} ModalAlignment;

typedef struct
{
    const char *title;
    int min_height;
    int min_width;
    float height_ratio;
    float width_ratio;
    ModalAlignment alignment;
    ThemeColor bg_color;
    ThemeColor border_color;
    ThemeColor title_color;
} ModalConfig;

WINDOW *create_modal(ModalConfig config, int *out_height, int *out_width);
void destroy_modal(WINDOW *win);

extern int max_rows;
extern int max_cols;

#endif
