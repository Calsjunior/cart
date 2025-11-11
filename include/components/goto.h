#ifndef GOTO_H
#define GOTO_H

#include "components/modal.h"

typedef struct
{
    const char *keys;
    const char *description;
} GotoHelp;

void draw_goto_help(void);

#endif
