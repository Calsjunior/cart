#ifndef FILE_BROWSER_H
#define FILE_BROWSER_H

#define _XOPEN_SOURCE 700

#include <math.h>
#include <unistd.h>

#include "colors.h"
#include "fs.h"
#include "icon.h"

#define PATH_LINE 2
#define STATUS_LINE 1
#define SCROLL_PADDING 2

void draw_file_browser(AppState *state, EntryList *list);
static void draw_path_line(AppState *state);
static void draw_status_line(AppState *state, EntryList *list);
static int draw_split_line(void);

static void adjust_scroll(int visible_lines, EntryList *list);
static int get_cursor_position(EntryList *list);

extern int max_rows;
extern int max_cols;

#endif
