#ifndef UI_H
#define UI_H

#include "fs.h"
#include "keymap.h"
#include "state.h"

void init_ui(void);
void draw_ui(DirList *list, AppState *state);
void handle_input(Action key, DirList *list, AppState *state);
void clean_ui(void);

#endif
