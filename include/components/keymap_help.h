#ifndef KEYMAP_HELP_H
#define KEYMAP_HELP_H

#include "components/modal.h"

typedef struct
{
    const char *keys;
    const char *description;
} KeyMapping;

typedef struct
{
    const char *name;
    KeyMapping *mappings;
    int num_mappings;
} KeySection;

void draw_keymap_help(void);

#endif
