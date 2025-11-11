#include "components/keymap_help.h"

static KeyMapping nav_mappings[] = {
    {" / , k/j", "Move cursor up/down"},
    {" / , h/l", "Enter/Exit directory"},
    {"PgUp/PgDn, ^U/^D", "Move half page"},
    {"g", "Go to"},
    {"G", "Jump to bottom"},
};

static KeyMapping file_mappings[] = {
    {"a", "Create file or folder"},
    {"d", "Delete file or folder"},
    {"v", "Open in editor"},

};

static KeyMapping view_mappings[] = {
    {".", "Toggle hidden files"},
};

static KeyMapping general_mappings[] = {
    {"?", "Show this help"},
    {"q", "Quit / Cancel"},
};

static KeySection sections[] = {
    {"Navigation", nav_mappings, sizeof(nav_mappings) / sizeof(nav_mappings[0])},
    {"File Operations", file_mappings, sizeof(file_mappings) / sizeof(file_mappings[0])},
    {"View", view_mappings, sizeof(view_mappings) / sizeof(view_mappings[0])},
    {"General", general_mappings, sizeof(general_mappings) / sizeof(general_mappings[0])},
};

static int num_sections = sizeof(sections) / sizeof(sections[0]);

void draw_keymap_help(void)
{
    ModalConfig config = {
        .title = "Keymaps",
        .min_height = 21,
        .min_width = 50,
        .height_ratio = 0.5,
        .width_ratio = 0.1,
        .alignment = MODAL_BOTTOM_RIGHT,
        .bg_color = THEME_MODAL_BG,
        .border_color = THEME_MODAL_BORDER,
        .title_color = THEME_MODAL_TITLE,
    };

    int height, width;
    WINDOW *help_win = create_modal(config, &height, &width);

    int current_row = 2;
    int key_col = 3;
    int desc_col = 25;

    for (int i = 0; i < num_sections; i++)
    {
        if (current_row >= height - 3)
        {
            break;
        }

        KeySection *section = &sections[i];
        WCOLOR_ON(help_win, THEME_KEYHELP_SECTION);
        mvwprintw(help_win, current_row, key_col, "%s", section->name);
        WCOLOR_OFF(help_win, THEME_KEYHELP_SECTION);
        current_row++;

        for (int i = 0; i < section->num_mappings; i++)
        {
            if (current_row >= height - 2)
            {
                break;
            }

            KeyMapping *mapping = &section->mappings[i];
            WCOLOR_ON(help_win, THEME_KEYHELP_KEYS);
            mvwprintw(help_win, current_row, key_col + 2, "%-8s", mapping->keys);
            WCOLOR_OFF(help_win, THEME_KEYHELP_KEYS);
            WCOLOR_ON(help_win, THEME_KEYHELP_DESC);
            mvwprintw(help_win, current_row, desc_col, "%s", mapping->description);
            WCOLOR_OFF(help_win, THEME_KEYHELP_DESC);
            current_row++;
        }
    }
    current_row++;

    wrefresh(help_win);
    destroy_modal(help_win);
}
