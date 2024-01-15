#include "s_hell.h"

/**
 * get_history_file - gets the history file
 * @data: parameter struct
 *
 * Return: allocated string containg history file
 */
char *get_history_file(s_cmd_data *data)
{
    char *buf, *dir;

    dir = fetch_shell_env(data, "HOME=");
    if (!dir)
        return NULL;
    buf = custom_malloc(custom_strlen(dir) + custom_strlen(HISTORY_FILE) + 2);
    if (!buf)
        return NULL;
    buf[0] = 0;
    custom_strcpy(buf, dir);
    custom_strcat(buf, "/");
    custom_strcat(buf, HISTORY_FILE);
    return buf;
}

/**
 * write_history - creates a file, or appends to an existing file
 * @data: the parameter struct
 *
 * Return: 1 on success, else -1
 */
int write_history(s_cmd_data *data)
{
    ssize_t fd;
    char *filename = get_history_file(data);
    s_list *node = NULL;

    if (!filename)
        return -1;

    fd = custom_open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
    custom_free(filename);
    if (fd == -1)
        return -1;
    for (node = data->cmd_log; node; node = node->next)
    {
        custom_putsfd(node->str, fd);
        custom_write_fd('\n', fd);
    }
    custom_write_fd(FLUSH_BUFFER, fd);
    custom_close(fd);
    return 1;
}

/**
 * read_history - reads history from file
 * @data: the parameter struct
 *
 * Return: histcount on success, 0 otherwise
 */
int read_history(s_cmd_data *data)
{
    int i, last = 0, linecount = 0;
    ssize_t fd, rdlen, fsize = 0;
    struct stat st;
    char *buf = NULL, *filename = get_history_file(data);

    if (!filename)
        return 0;

    fd = custom_open(filename, O_RDONLY);
    custom_free(filename);
    if (fd == -1)
        return 0;
    if (!custom_fstat(fd, &st))
        fsize = st.st_size;
    if (fsize < 2)
        return 0;
    buf = custom_malloc(custom_strlen(dir) + 1);
    if (!buf)
        return 0;
    rdlen = custom_read(fd, buf, fsize);
    buf[fsize] = 0;
    if (rdlen <= 0)
        return custom_free(buf), 0;
    custom_close(fd);
    for (i = 0; i < fsize; i++)
        if (buf[i] == '\n')
        {
            buf[i] = 0;
            build_history_list(data, buf + last, linecount++);
            last = i + 1;
        }
    if (last != i)
        build_history_list(data, buf + last, linecount++);
    custom_free(buf);
    data->hist_count = linecount;
    while (data->hist_count-- >= MAX_HISTORY)
        delete_custom_node_at_index(&(data->cmd_log), 0);
    renumber_custom_history(data);
    return data->hist_count;
}

/**
 * build_history_list - adds entry to a history linked list
 * @data: Structure containing potential arguments. Used to maintain
 * @buf: buffer
 * @linecount: the history linecount, histcount
 *
 * Return: Always 0
 */
int build_history_list(s_cmd_data *data, char *buf, int linecount)
{
    s_list *node = NULL;

    if (data->cmd_log)
        node = data->cmd_log;
    custom_add_node(&node, buf, linecount);

    if (!data->cmd_log)
        data->cmd_log = node;
    return 0;
}

/**
 * renumber_history - renumbers the history linked list after changes
 * @data: Structure containing potential arguments. Used to maintain
 *
 * Return: the new histcount
 */
int renumber_history(s_cmd_data *data)
{
    s_list *node = data->cmd_log;
    int i = 0;

    while (node)
    {
        node->num = i++;
        node = node->next;
    }
    return data->hist_count = i;
}

