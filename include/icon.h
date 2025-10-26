#ifndef ICON_H
#define ICON_H

#include "fs.h"

typedef struct
{
    const char *extension;
    const char *icon;
} EntryTypeExt;

const char *get_file_icon(const char *filename, EntryType type);

#endif
