#ifndef TEXT_UTIL_H
#define TEXT_UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *helper_set_full_path(char *buffer, size_t buffer_size, const char *dir_path, const char *filename);
void truncate_start(char *dest, size_t dest_size, const char *src, int max_width);
void truncate_middle(char *dest, size_t dest_size, const char *src, int max_width);
void truncate_path(char *dest, size_t dest_size, const char *src, int max_width);

#endif
