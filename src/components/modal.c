#include "components/modal.h"

WINDOW *create_modal(ModalConfig config, int *out_height, int *out_width)
{
    int height = (int) (max_rows * config.height_ratio);
    int width = (int) (max_cols * config.width_ratio);

    if (height < config.min_height)
    {
        height = config.min_height;
    }
    if (width < config.min_width)
    {
        width = config.min_width;
    }

    // Ensure UI fits on the screen
    if (height > max_rows)
    {
        height = max_rows - 2;
    }
    if (width > max_cols)
    {
        width = max_cols - 2;
    }

    // Center to screen
    int start_y, start_x;

    switch (config.alignment)
    {
        case MODAL_CENTER:
            start_y = (max_rows - height) / 2;
            start_x = (max_cols - width) / 2;
            break;

        case MODAL_TOPHALF_CENTER:
            start_y = (max_rows - height) / 4;
            start_x = (max_cols - width) / 2;
            break;

        case MODAL_BOTTOM_RIGHT:
            start_y = max_rows - height - 1;
            start_x = max_cols - width;
            break;
    }

    WINDOW *win = newwin(height, width, start_y, start_x);
    if (config.bg_color != 0)
    {
        wbkgd(win, COLOR_PAIR(config.bg_color) | ' ');
    }

    if (config.border_color != 0)
    {
        WCOLOR_ON(win, config.border_color);
        box(win, 0, 0);
        WCOLOR_OFF(win, config.border_color);
    }
    else
    {
        box(win, 0, 0);
    }

    if (config.title != NULL)
    {
        int title_x = (width - strlen(config.title)) / 2;

        if (config.title_color != 0)
        {
            WCOLOR_ON(win, config.title_color);
        }
        mvwprintw(win, 0, title_x, " %s ", config.title);
        WCOLOR_OFF(win, config.title_color);
    }

    if (out_height != NULL)
    {
        *out_height = height;
    }
    if (out_width != NULL)
    {
        *out_width = width;
    }

    return win;
}

void destroy_modal(WINDOW *win)
{
    delwin(win);
    refresh();
}
