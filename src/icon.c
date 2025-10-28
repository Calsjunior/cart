#include <string.h>

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
    {".c", "󰙱 "},
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

const char *get_file_icon(const char *filename, EntryType type)
{
    if (type == ENTRY_DIR)
    {
        return "󰉋 ";
    }

    const char *ext = strrchr(filename, '.');
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
