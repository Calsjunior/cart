#include "text_utils.h"

char *helper_set_full_path(char *buffer, size_t buffer_size, const char *dir_path, const char *filename)
{
    if (strcmp(dir_path, "/") == 0)
    {
        snprintf(buffer, buffer_size, "/%s", filename);
    }
    else
    {
        snprintf(buffer, buffer_size, "%s/%s", dir_path, filename);
    }
    return buffer;
}

void truncate_start(char *dest, size_t dest_size, const char *src, int max_width)
{
    int src_len = strlen(src);
    if (src_len <= max_width)
    {
        snprintf(dest, dest_size, "%s", src);
        return;
    }

    int start_pos = src_len - max_width - 3;
    snprintf(dest, dest_size, "…%s,", src + start_pos);
}

void truncate_middle(char *dest, size_t dest_size, const char *src, int max_width)
{
    int src_len = strlen(src);
    if (src_len <= max_width)
    {
        snprintf(dest, dest_size, "%s", src);
        return;
    }

    int left_len = (max_width - 3) / 2;
    int right_len = max_width - 3 - left_len;

    if (left_len > src_len)
    {
        left_len = src_len;
    }
    if (right_len > src_len)
    {
        right_len = src_len;
    }

    snprintf(dest, dest_size, "%.*s…%.*s", left_len, src, right_len, src + src_len - right_len);
}

void truncate_path(char *dest, size_t dest_size, const char *src, int max_width)
{
    int src_len = strlen(src);
    if (src_len <= max_width)
    {
        snprintf(dest, dest_size, "%s", src);
        return;
    }

    const char *last_slash = strrchr(src, '/');
    if (last_slash == NULL)
    {
        truncate_middle(dest, dest_size, src, max_width);
        return;
    }

    const char *filename = last_slash + 1;
    int filename_len = strlen(filename);
    if (filename_len >= max_width - 4)
    {
        truncate_middle(dest, dest_size, src, max_width);
        return;
    }

    int path_space = max_width - filename_len - 2;
    if (path_space < 1)
    {
        snprintf(dest, dest_size, "…/%s", filename);
        return;
    }

    snprintf(dest, dest_size, "%.*s…/%s", path_space, src, filename);
}
