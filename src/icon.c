#include <string.h>

#include "colors.h"
#include "fs.h"
#include "icon.h"

static EntryTypeExt ext_list[] = {

    // Documents
    {".txt", " "},
    {".pdf", " "},
    {".doc", " "},
    {".csv", " "},
    {".md", "󰯂 "},

    // Media
    {".png", " "},
    {".jpg", " "},

    {".mp3", " "},
    {".wav", " "},
    {".mp4", " "},
    {".mov", " "},

    // Languages
    {".c", "󰙱 ", THEME_CODE_C},
    {".cpp", "󰙲 "},
    {".py", " "},
    {".sh", " "},
    {".json", " "},
    {".lua", "󰢱 "},
    {".h", "󰜕 "},
    {".o", " "},

    // Compressed
    {".zip", "󰛫 "},
    {".rar", "󰛫 "},
    {".tar", "󰛫 "},
    {".gz", "󰛫 "},

    // General
    {".iso", "󰗮 "},
};

static int num_ext_type = sizeof(ext_list) / sizeof(ext_list[0]);

const char *get_entry_icon(const char *entryname, EntryType type)
{
    if (type == ENTRY_DIR)
    {
        return "󰉋 ";
    }

    if (type == ENTRY_SYMLINK_DIR)
    {
        return " ";
    }

    if (type == ENTRY_SYMLINK_FILE)
    {
        return " ";
    }

    const char *ext = strrchr(entryname, '.');
    if (ext == NULL)
    {
        return " ";
    }

    for (int i = 0; i < num_ext_type; i++)
    {
        if (strcmp(ext, ext_list[i].extension) == 0)
        {
            return ext_list[i].icon;
        }
    }

    return " ";
}

ThemeColor get_entry_color(const char *entryname, EntryType type)
{
    if (type == ENTRY_DIR)
    {
        return THEME_DIR;
    }

    const char *ext = strrchr(entryname, '.');
    if (ext == NULL)
    {
        return THEME_FILE;
    }

    for (int i = 0; i < num_ext_type; i++)
    {
        if (strcmp(ext, ext_list[i].extension) == 0)
        {
            return ext_list[i].color;
        }
    }
    return THEME_FILE;
}
