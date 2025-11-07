#include "components/create.h"

void draw_create_entry_prompt(AppState *state)
{
    ModalConfig config = {
        .title = "Create entry",
        .min_height = 7,
        .min_width = 45,
        .height_ratio = 0.1,
        .width_ratio = 0.3,
        .alignment = MODAL_TOPHALF_CENTER,
        .bg_color = THEME_MODAL_BG,
        .border_color = THEME_MODAL_BORDER,
        .title_color = THEME_MODAL_TITLE,
    };

    int height, width;
    WINDOW *create_win = create_modal(config, &height, &width);

    // Prints input
    int input_y = height / 2;
    int input_x = height / 2;
    int max_input_width = width - 6;

    int scroll_offset = 0;
    if (state->input_pos > max_input_width)
    {
        scroll_offset = state->input_pos - max_input_width;
    }

    int display_len = state->input_pos - scroll_offset;
    if (display_len > max_input_width)
    {
        display_len = max_input_width;
    }

    wmove(create_win, input_y, input_x);

    mvwprintw(create_win, input_y, input_x, "%.*s", display_len, state->input + scroll_offset);

    int cursor_x = input_x + (state->input_pos - scroll_offset);
    wmove(create_win, input_y, cursor_x);

    wrefresh(create_win);
}
