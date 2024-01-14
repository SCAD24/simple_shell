#include "s_hell.h"

/**
 * shell_history - Prints the command history list.
 * @data: Pointer to a struct cmd_data containing the command history list.
 * Return: Always 0.
 */
int shell_history(s_cmd_data *data)
{
    custom_print_list_str(data->cmd_log);
    return 0;
}

/**
 * unset_custom_env - Unsets environment variable to string.
 * @data: Parameter struct.
 * @str: The string environment variable.
 *
 * Return: Always 0 on success, 1 on error.
 */
int unset_custom_env(s_cmd_data *data, char *str)
{
    char *p, c;
    int ret;

    p = custom_strchr(str, '=');
    if (!p)
        return 1;
    c = *p;
    *p = 0;
    ret = delete_custom_node_at_index(&(data->tsh_env),
            get_custom_node_index(data->tsh_env, node_starts_with(data->tsh_env, str, -1)));
    *p = c;
    return ret;
}

/**
 * set_custom_env - Sets environment variable to string.
 * @data: Parameter struct.
 * @str: The string environment variable.
 *
 * Return: Always 0 on success, 1 on error.
 */
int set_custom_env(s_cmd_data *data, char *str)
{
    char *p;

    p = custom_strchr(str, '=');
    if (!p)
        return 1;
    if (!*++p)
        return unset_custom_env(data, str);

    unset_custom_env(data, str);
    return (add_custom_node_end(&(data->tsh_env), str, 0) == NULL);
}

/**
 * print_custom_env - Prints an environment variable string.
 * @node: The environment variable node.
 *
 * Return: Always 0 on success, 1 on error.
 */
int print_custom_env(s_list *node)
{
    char *p = NULL, *a = NULL;

    if (node)
    {
        p = custom_strchr(node->str, '=');
        for (a = node->str; a <= p; a++)
            custom_putchar(*a);
        custom_putchar('\'');
        custom_puts(p + 1);
        custom_puts("'\n");
        return 0;
    }
    return 1;
}

/**
 * my_custom_env - Mimics the env builtin.
 * @data: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * Return: Always 0.
 */
int my_custom_env(s_cmd_data *data)
{
    int i = 0;
    char *p = NULL;
    s_list *node = NULL;

    if (data->arg_count == 1)
    {
        node = data->tsh_env;
        while (node)
        {
            print_custom_env(node);
            node = node->next;
        }
        return 0;
    }

    for (i = 1; data->argv[i]; i++)
    {
        p = custom_strchr(data->argv[i], '=');
        if (p)
            set_custom_env(data, data->argv[i]);
        else
            print_custom_env(node_starts_with(data->tsh_env, data->argv[i], '='));
    }

    return 0;
}

