#include "s_hell.h"

/**
 * run_loop - main shell loop
 * @data: a pointer to an s_cmd_data struct containing shell information
 * @av: double pointer to an array of strings containing command line arguments
 *
 * Return: 0 on success, 1 on error, or error code
 */
int run_loop(s_cmd_data *data, char **av)
{
    ssize_t value = 0;
    int cmd_res = 0;

    while (value != -1 && cmd_res != -2)
    {
        clear_shell_data(data);
        if (interact_shell(data))
            custom_puts("$ ");
        custom_fd(FLUSH_BUFFER);
        value = fetch_input(data);
        if (value != -1)
        {
            setup_shell_data(data, av);
            cmd_res = distinct_exe(data);
            if (cmd_res == -1)
                varied_path(data);
        }
        else if (interact_shell(data))
            custom_putchar('\n');
        release_shell_data(data, 0);
    }
    write_custom_history(data);
    release_shell_data(data, 1);
    if (!interact_shell(data) && data->tsh_status)
        exit(data->tsh_status);
    if (cmd_res == -2)
    {
        if (data->xcode == -1)
            exit(data->tsh_status);
        exit(data->xcode);
    }
    return (cmd_res);
}

/**
 * distinct_exe - Finds and executes the appropriate built-in command
 * @data: Pointer to the s_cmd_data struct containing information about the
 *        command to be executed.
 * Return: -1 if built-in not found,
 *         0 if built-in executed successfully,
 *         1 if built-in found but not successful,
 *         -2 if built-in signals exit()
 */
int distinct_exe(s_cmd_data *data)
{
    int x, retval = -1;
    s_builtin_reg builtin_entry[] = {
        {"exit", exit_shell},
        {"env", my_custom_env},
        {"help", help_cd},
        {"history", shell_history},
        {"setenv", my_custom_setenv},
        {"unsetenv", my_custom_unsetenv},
        {"cd", change_directory},
        {"alias", my_custom_alias},
        {NULL, NULL}};

    for (x = 0; builtin_entry[x].type; x++)
        if (custom_strcmp(data->argv[0], builtin_entry[x].type) == 0)
        {
            data->index++;
            retval = builtin_entry[x].func(data);
            break;
        }
    return (retval);
}

/**
 * varied_path - searches for a command in PATH
 * @data: Pointer to the command data struct
 *
 * Return: void
 */
void varied_path(s_cmd_data *data)
{
    char *course = NULL;
    int x, t;

    data->course = data->argv[0];
    if (data->line_count == 1)
    {
        data->index++;
        data->line_count = 0;
    }
    for (x = 0, t = 0; data->arg[x]; x++)
        if (!custom_delim(data->arg[x], " \t\n"))
            t++;
    if (!t)
        return;

    course = absolute_route(data, fetch_shell_env(data, "PATH="), data->argv[0]);
    if (course)
    {
        data->course = course;
        replaced_fork(data);
    }
    else
    {
        if ((interact_shell(data) || fetch_shell_env(data, "PATH=") || data->argv[0][0] == '/') && distinct_exe(data, data->argv[0]))
            replaced_fork(data);
        else if (*(data->arg) != '\n')
        {
            data->tsh_status = 127;
            display_custom_error(data, "not found\n");
        }
    }
}

/**
 * replaced_fork - Creates a child process using fork() and executes a command in it
 * @data: A pointer to a struct containing information about the command to be
 *        executed
 *
 * Return: void
 */
void replaced_fork(s_cmd_data *data)
{
   

