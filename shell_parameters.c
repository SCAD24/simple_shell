#include "s_hell.h"

/**
 * is_chain - test if current char in buffer is a chain delimiter
 * @data: the parameter struct
 * @buf: the char buffer
 * @p: address of current position in buf
 *
 * Return: 1 if chain delimiter, 0 otherwise
 */
int is_chain(s_cmd_data *data, char *buf, size_t *p)
{
    size_t j = *p;

    if (buf[j] == '|' && buf[j + 1] == '|')
    {
        buf[j] = 0;
        j++;
        data->cmd_buf_type = CMD_DIFFERENT;
    }
    else if (buf[j] == '&' && buf[j + 1] == '&')
    {
        buf[j] = 0;
        j++;
        data->cmd_buf_type = CMD_VARIED;
    }
    else if (buf[j] == ';') /* found end of this command */
    {
        buf[j] = 0; /* replace semicolon with null */
        data->cmd_buf_type = CMD_REPLACED;
    }
    else
        return (0);
    *p = j;
    return (1);
}

/**
 * check_chain - checks if we should continue chaining based on the last status
 * @data: the parameter struct
 * @buf: the char buffer
 * @p: address of current position in buf
 * @i: starting position in buf
 * @len: length of buf
 *
 * Return: Void
 */
void check_chain(s_cmd_data *data, char *buf, size_t *p, size_t i, size_t len)
{
    size_t j = *p;

    if (data->cmd_buf_type == CMD_VARIED)
    {
        if (data->tsh_status)
        {
            buf[i] = 0;
            j = len;
        }
    }
    if (data->cmd_buf_type == CMD_DIFFERENT)
    {
        if (!data->tsh_status)
        {
            buf[i] = 0;
            j = len;
        }
    }

    *p = j;
}

/**
 * s_hell_replace_alias - replaces aliases in the tokenized string
 * @data: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int s_hell_replace_alias(s_cmd_data *data)
{
    int i;
    s_list *node;
    char *p;

    for (i = 0; i < 10; i++)
    {
        node = custom_node_starts_with(data->tsh_alias, data->argv[0], '=');
        if (!node)
            return (0);
        custom_free(data->argv[0]);
        p = custom_strchr(node->str, '=');
        if (!p)
            return (0);
        p = custom_strdup(p + 1);
        if (!p)
            return (0);
        data->argv[0] = p;
    }
    return (1);
}

/**
 * s_hell_replace_vars - replaces variables in the tokenized string
 * @data: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int s_hell_replace_vars(s_cmd_data *data)
{
    int i = 0;
    s_list *node;

    for (i = 0; data->argv[i]; i++)
    {
        if (data->argv[i][0] != '$' || !data->argv[i][1])
            continue;

        if (!custom_strcmp(data->argv[i], "$?"))
        {
            s_hell_replace_string(&(data->argv[i]),
                                  custom_strdup(custom_convert_number(data->tsh_status, 10, 0)));
            continue;
        }
        if (!custom_strcmp(data->argv[i], "$$"))
        {
            s_hell_replace_string(&(data->argv[i]),
                                  custom_strdup(custom_convert_number(getpid(), 10, 0)));
            continue;
        }
        node = custom_node_starts_with(data->tsh_env, &data->argv[i][1], '=');
        if (node)
        {
            s_hell_replace_string(&(data->argv[i]),
                                  custom_strdup(custom_strchr(node->str, '=') + 1));
            continue;
        }
        s_hell_replace_string(&data->argv[i], custom_strdup(""));
    }
    return (0);
}

/**
 * s_hell_replace_string - replaces string
 * @old: address of old string
 * @new: new string
 *
 * Return: 1 if replaced, 0 otherwise
 */
int s_hell_replace_string(char **old, char *new)
{
    custom_free(*old);
    *old = new;
    return (1);
}

