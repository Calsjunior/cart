#ifndef UI_H
#define UI_H

#include <ncurses.h>

#include "colors.h"
#include "components/create.h"
#include "components/delete.h"
#include "components/file_browser.h"
#include "components/keymap_help.h"
#include "fs.h"
#include "icon.h"
#include "keymap.h"
#include "state.h"
#include "text_utils.h"

void init_ui(void);
void draw_ui(AppState *state, EntryList *list);
void handle_input(Action key, AppState *state, Stack *stack, EntryList *list);
void clean_ui(void);

#endif
