#include "s_hell.h"

/**
 * get_custom_environ - Returns the string array copy of the custom environment.
 * @data: Structure containing potential arguments.
 *
 * Return: String array representing the custom environment.
 */
char **get_custom_environ(s_cmd_data *data)
{
    if (!data->environ || data->env_changed) {
        data->environ = list_to_custom_strings(data->tsh_env);
        data->env_changed = 0;
    }

    return data->environ;
}

/**
 * unset_custom_env - Removes an environment variable from the custom environment.
 * @data: Structure containing potential arguments.
 * @var: The name of the environment variable to remove.
 *
 * Return: 1 on successful removal, 0 otherwise.
 */
int unset_custom_env(s_cmd_data *data, char *var)
{
    s_list *node = data->tsh_env;
    size_t i = 0;
    char *p;

    if (!node || !var)
        return 0;

    while (node) {
        p = match_prefix(node->str, var);
        if (p && *p == '=') {
            data->env_changed = delete_custom_node_at_index(&(data->tsh_env), i);
            i = 0;
            node = data->tsh_env;
            continue;
        }
        node = node->next;
        i++;
    }

    return data->env_changed;
}

/**
 * initialize_custom_env - Sets an environment variable in the custom environment.
 * @data: A pointer to the cmd_data struct containing environment variables.
 * @var: The name of the environment variable to set.
 * @value: The value to set for the environment variable.
 *
 * Return: 0 on success, 1 on failure.
 */
int initialize_custom_env(s_cmd_data *data, char *var, char *value)
{
    char *buf = NULL;
    s_list *node;
    char *p;

    if (!var || !value)
        return 0;

    buf = malloc(custom_strlen(var) + custom_strlen(value) + 2);
    if (!buf)
        return 1;

    custom_strcpy(buf, var);
    custom_strcat(buf, "=");
    custom_strcat(buf, value);

    node = data->tsh_env;

    while (node) {
        p = match_prefix(node->str, var);
        if (p && *p == '=') {
            custom_free_list(&(data->tsh_env));
            add_custom_node_end(&(data->tsh_env), buf, 0);
            free(buf);
            data->env_changed = 1;
            return 0;
        }
        node = node->next;
    }

    add_custom_node_end(&(data->tsh_env), buf, 0);
    free(buf);
    data->env_changed = 1;
    return 0;
}

