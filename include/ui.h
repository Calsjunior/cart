#ifndef UI_H
#define UI_H

#include "fs.h"
#include "keymap.h"
#include "state.h"

void init_ui(void);
void draw_ui(EntryList *list);
void handle_input(Action key, AppState *state, Stack *stack, EntryList *list);
void clean_ui(void);

#endif
