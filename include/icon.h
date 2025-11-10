#ifndef ICON_H
#define ICON_H

#include <string.h>

#include "colors.h"
#include "fs.h"

typedef struct
{
    const char *extension;
    const char *icon;
    ThemeColor color;
} EntryTypeExt;

const char *get_entry_icon(const char *entryname, EntryType type);
ThemeColor get_entry_icon_color(const char *entryname, EntryType type);
ThemeColor get_entry_color(const char *entryname, EntryType type);

#endif
