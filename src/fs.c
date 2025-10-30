#include "fs.h"

static void add_entry_node(char *name, EntryType type, EntryList *list);
static void delete_directory_recursively(const char *path);
static int remove_callback(const char *path, const struct stat *sb, int typeflag, struct FTW *ftwbuf);

char *helper_set_full_path(char *buffer, size_t buffer_size, char *name, AppState *state);

void list_dir(AppState *state, EntryList *list)
{
    // Initialize the doubly linked list
    free_list(list);

    DIR *dir = opendir(state->dir_path);
    if (dir == NULL)
    {
        clear();
        mvprintw(0, 0, "Directory cannot be openned.");
        return;
    }

    struct dirent *dir_entry;
    while ((dir_entry = readdir(dir)) != NULL)
    {
        if (strcmp(dir_entry->d_name, ".") == 0 || strcmp(dir_entry->d_name, "..") == 0)
        {
            continue;
        }

        if (!state->show_hidden && dir_entry->d_name[0] == '.')
        {
            continue;
        }

        char full_path[PATH_MAX];
        helper_set_full_path(full_path, sizeof(full_path), dir_entry->d_name, state);

        struct stat entry_stat;
        struct stat target_stat;
        if (lstat(full_path, &entry_stat) < 0)
        {
            continue;
        }

        EntryType type;
        if (S_ISDIR(entry_stat.st_mode))
        {
            type = ENTRY_DIR;
        }
        else if (S_ISREG(entry_stat.st_mode))
        {
            type = ENTRY_FILE;
        }
        else if (S_ISLNK(entry_stat.st_mode))
        {
            if (stat(full_path, &target_stat) < 0)
            {
                continue;
            }

            if (S_ISDIR(target_stat.st_mode))
            {
                type = ENTRY_SYMLINK_DIR;
            }
            else if (S_ISREG(target_stat.st_mode))
            {
                type = ENTRY_SYMLINK_FILE;
            }
        }
        add_entry_node(dir_entry->d_name, type, list);
        list->count_entries++;
    }

    closedir(dir);
}

void free_list(EntryList *list)
{
    EntryNode *current = list->head;
    while (current != NULL)
    {
        EntryNode *next = current->next;
        free(current->name);
        free(current);
        current = next;
    }
    list->head = NULL;
    list->tail = NULL;
    list->cursor = NULL;
    list->count_entries = 0;
    list->scroll_offset = 0;
}

void subdir_stack_push(AppState *state, Stack *stack, EntryList *list)
{
    StackNode *stack_node = malloc(sizeof(StackNode));
    if (stack_node == NULL)
    {
        return;
    }

    stack_node->path = strdup(state->dir_path);

    if (list->cursor == NULL)
    {
        stack_node->cursor_name = NULL;
        stack_node->scroll_offset = 0;
    }
    else
    {
        stack_node->cursor_name = strdup(list->cursor->name);
        stack_node->scroll_offset = list->scroll_offset;
    }

    stack_node->next = stack->top;
    stack->top = stack_node;
}

void subdir_stack_pop(AppState *state, Stack *stack, EntryList *list)
{
    if (stack->top == NULL)
    {
        return;
    }

    StackNode *stack_node = stack->top;
    stack->top = stack_node->next;

    free(state->dir_path);
    state->dir_path = strdup(stack_node->path);

    if (state->cursor_name != NULL)
    {
        free(state->cursor_name);
    }

    if (stack_node->cursor_name != NULL)
    {
        state->cursor_name = strdup(stack_node->cursor_name);
    }
    else
    {
        state->cursor_name = NULL;
    }
    state->scroll_offset = stack_node->scroll_offset;

    free(stack_node->path);
    free(stack_node);
}

void free_stack(Stack *stack)
{
    StackNode *current_stack_node = stack->top;
    while (current_stack_node != NULL)
    {
        StackNode *next = current_stack_node->next;
        free(current_stack_node->path);
        free(current_stack_node);
        current_stack_node = next;
    }
    stack->top = NULL;
}

void restore_cursor(AppState *state, EntryList *list)
{
    if (state->cursor_name == NULL)
    {
        return;
    }

    list->scroll_offset = state->scroll_offset;

    // Check if new directory list's name matches name that is pushed to the stack
    for (EntryNode *current = list->head; current != NULL; current = current->next)
    {
        if (strcmp(current->name, state->cursor_name) == 0)
        {
            list->cursor = current;
            break;
        }
    }
    free(state->cursor_name);
    state->cursor_name = NULL;
}

void navigate_subdir(AppState *state, EntryList *list)
{
    char new_path[PATH_MAX];
    helper_set_full_path(new_path, sizeof(new_path), list->cursor->name, state);

    free(state->dir_path);
    state->dir_path = strdup(new_path);
}

void navigate_root(AppState *state)
{
    char *slash = strrchr(state->dir_path, '/');
    if (slash != NULL && slash != state->dir_path)
    {
        *slash = '\0';
        return;
    }
    strcpy(state->dir_path, "/");
}

void delete_entry(AppState *state, EntryList *list)
{
    if (list->cursor->next != NULL)
    {
        state->cursor_name = strdup(list->cursor->next->name);
    }
    else if (list->cursor->next == NULL && list->cursor->prev == NULL)
    {
        state->cursor_name = NULL;
    }
    else if (list->cursor->next == NULL)
    {
        state->cursor_name = strdup(list->cursor->prev->name);
    }

    char full_path[PATH_MAX];
    helper_set_full_path(full_path, sizeof(full_path), list->cursor->name, state);

    if (remove(full_path) == 0)
    {
        return;
    }

    if (list->cursor->type == ENTRY_DIR)
    {
        delete_directory_recursively(full_path);
        return;
    }
}

void open_entry(AppState *state, EntryList *list)
{
    if (list->cursor == NULL)
    {
        return;
    }

    if (state->cursor_name != NULL)
    {
        free(state->cursor_name);
    }
    state->cursor_name = strdup(list->cursor->name);
    state->scroll_offset = list->scroll_offset;

    const char *editor = getenv("EDITOR");
    if (editor == NULL)
    {
        editor = getenv("VISUAL");
    }

    if (editor == NULL)
    {
        editor = "vim";
    }

    char full_path[PATH_MAX];
    helper_set_full_path(full_path, sizeof(full_path), list->cursor->name, state);

    def_prog_mode();
    endwin();

    char command[512];
    snprintf(command, sizeof(command), "%s '%s'", editor, full_path);
    system(command);

    reset_prog_mode();
    refresh();
}

// TODO: add nested directory/files logic
void create_entry(char *name, AppState *state)
{
    int name_len = strlen(name);
    if (name == NULL || name_len == 0)
    {
        return;
    }

    char full_path[PATH_MAX];
    helper_set_full_path(full_path, sizeof(full_path), name, state);

    bool is_directory = false;
    if (name_len > 0 && name[name_len - 1] == '/')
    {
        mkdir(full_path, 0777);
        is_directory = true;
    }
    else
    {
        FILE *file = fopen(full_path, "w");
        if (file == NULL)
        {
            return;
        }
        fclose(file);
    }

    if (state->cursor_name != NULL)
    {
        free(state->cursor_name);
    }

    if (is_directory)
    {
        name[name_len - 1] = '\0';
    }
    state->cursor_name = strdup(name);
    state->scroll_offset = 0;
}

static void add_entry_node(char *name, EntryType type, EntryList *list)
{
    EntryNode *node = malloc(sizeof(EntryNode));
    if (node == NULL)
    {
        free_list(list);
        return;
    }
    node->name = strdup(name);
    node->type = type;
    node->next = NULL;
    node->prev = NULL;

    // If list is empty
    if (list->head == NULL)
    {
        list->head = list->tail = list->cursor = node;
        return;
    }

    EntryNode *current = list->head;
    while (current != NULL)
    {
        bool insert_before = FALSE;

        // Determine if entry is folder or files
        bool node_is_dir = (node->type == ENTRY_DIR || node->type == ENTRY_SYMLINK_DIR);
        bool current_is_dir = (current->type == ENTRY_DIR || current->type == ENTRY_SYMLINK_DIR);

        // Sort directory before file
        if (node_is_dir && !current_is_dir)
        {
            insert_before = TRUE;
        }

        // If same type then sort in alphabetical order
        else if (node_is_dir == current_is_dir && strcasecmp(node->name, current->name) < 0)
        {
            insert_before = TRUE;
        }

        if (insert_before)
        {
            node->next = current;
            node->prev = current->prev;

            if (current->prev == NULL)
            {
                current->prev = node;
                list->head = node;
                list->cursor = node;
                return;
            }
            current->prev->next = node;
            current->prev = node;
            return;
        }
        current = current->next;
    }
    node->prev = list->tail;
    list->tail->next = node;
    list->tail = node;
}

static void delete_directory_recursively(const char *path)
{
    nftw(path, remove_callback, 64, FTW_DEPTH | FTW_PHYS);
}

// Remove_callback needed for nftw per documentation
static int remove_callback(const char *path, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
    int result = remove(path);

    if (result != 0)
    {
        return -1;
    }

    return result;
}

char *helper_set_full_path(char *buffer, size_t buffer_size, char *name, AppState *state)
{
    if (strcmp(state->dir_path, "/") == 0)
    {
        snprintf(buffer, buffer_size, "/%s", name);
    }
    else
    {
        snprintf(buffer, buffer_size, "%s/%s", state->dir_path, name);
    }
    return buffer;
}
