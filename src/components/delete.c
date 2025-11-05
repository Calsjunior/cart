#include "components/delete.h"

void draw_delete_entry_prompt(AppState *state, EntryList *list)
{
    ModalConfig config = {
        .title = "Delete this entry?",
        .min_height = 7,
        .min_width = 45,
        .height_ratio = 0.2,
        .width_ratio = 0.3,
        .alignment = MODAL_TOPHALF_CENTER,
        .bg_color = THEME_MODAL_BG,
        .border_color = THEME_MODAL_BORDER,
        .title_color = THEME_DELETE_TITLE,
    };

    int height, width;
    WINDOW *delete_win = create_modal(config, &height, &width);

    char full_path[PATH_MAX];
    helper_set_full_path(full_path, sizeof(full_path), state->dir_path, list->cursor->name);

    // Truncate the beginning of the path, width - 6 seems to works well
    char display_path[512];
    int max_path_width = width - 6;
    truncate_start(display_path, sizeof(display_path), full_path, max_path_width);

    // Prints the path name
    int path_y = 3;
    int path_x = height / 2;
    mvwprintw(delete_win, path_y, path_x, "%s", display_path);

    // Prints yes/no option
    const char *yes_option = "[Y]es";
    const char *no_option = "[N]o";

    int spacing = 15;
    int total_width = strlen(yes_option) + strlen(no_option) + spacing;
    int option_y = 6;
    int option_x = (width - total_width) / 2;

    WCOLOR_ON(delete_win, THEME_DELETE_OPTION);
    mvwprintw(delete_win, option_y, option_x, " %s ", yes_option);
    mvwprintw(delete_win, option_y, option_x + strlen(yes_option) + spacing, " %s ", no_option);
    WCOLOR_OFF(delete_win, THEME_DELETE_OPTION);

    wrefresh(delete_win);
    destroy_modal(delete_win);
}
