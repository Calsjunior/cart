#ifndef UI_H
#define UI_H

#include "fs.h"
#include "keymap.h"
#include "state.h"

#define HALF_PAGE 17

void init_ui(void);
void draw_ui(AppState *state, EntryList *list);
void keymap_help(void);
void handle_input(Action key, AppState *state, Stack *stack, EntryList *list);
void clean_ui(void);

#endif
