#ifndef UI_H
#define UI_H

#define _XOPEN_SOURCE 700
#define _DEFAUT_SOURCE

#include <ncurses.h>
#include <string.h>
#include <unistd.h>

#include "colors.h"
#include "fs.h"
#include "icon.h"
#include "keymap.h"
#include "state.h"

#define MIN_ROWS 20
#define MIN_COLS 60

#define PATH_LINE 1
#define STATUS_LINE 1
#define SCROLL_PADDING 2

void init_ui(void);
void draw_ui(AppState *state, EntryList *list);
void handle_input(Action key, AppState *state, Stack *stack, EntryList *list);
void clean_ui(void);

#endif
