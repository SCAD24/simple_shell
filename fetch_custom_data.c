#include "s_hell.h"

/**
 * clear_shell_data - Initializes the cmd_data struct.
 * @data: Pointer to the cmd_data struct to be initialized.
 */
void clear_shell_data(s_cmd_data *data)
{
    // Set initial values for cmd_data struct
    data->arg = NULL;
    data->argv = NULL;
    data->course = NULL;
    data->arg_count = 0;
}

/**
 * setup_shell_data - Sets the data for the cmd_data struct based on the provided arguments.
 * @data: Pointer to the cmd_data struct to be populated.
 * @av: Array of strings containing the command arguments.
 */
void setup_shell_data(s_cmd_data *data, char **av)
{
    int i = 0;

    // Set the filename based on the first argument
    data->filename = av[0];

    if (data->arg)
    {
        // Tokenize the argument string and set the argv array
        data->argv = unique_tokens(data->arg, " \t");

        if (!data->argv)
        {
            // If tokenization fails, manually set the argv array
            data->argv = malloc(sizeof(char *) * 2);

            if (data->argv)
            {
                data->argv[0] = custom_strdup(data->arg);
                data->argv[1] = NULL;
            }
        }

        // Count the number of arguments in argv
        for (i = 0; data->argv && data->argv[i]; i++)
            ;

        // Set the argument count
        data->arg_count = i;

        // Perform alias and variable replacements
        replaced_fork(data);
        varied_path(data);
    }
}

/**
 * release_shell_data - Frees the memory allocated for cmd_data struct.
 * @data: Pointer to the cmd_data struct to be freed.
 * @all: Flag to indicate if all memory should be freed.
 */
void release_shell_data(s_cmd_data *data, int all)
{
    // Free the argv array
    free_custom(data->argv);
    data->argv = NULL;
    
    // Reset some fields to default values
    data->course = NULL;

    if (all)
    {
        // Free additional memory based on the 'all' flag
        if (!data->cmd_buf)
            free(data->arg);

        // Free linked lists
        if (data->tsh_env)
            custom_free_list(&(data->tsh_env));
        if (data->cmd_log)
            custom_free_list(&(data->cmd_log));

        // Reset and free environment variables
        ffree(data->environ);
        data->environ = NULL;

        // Deallocate memory for command buffer
        custom_dealloc((void **)data->cmd_buf);

        // Close file descriptor if it's greater than 2
        if (data->readfd > 2)
            close(data->readfd);

        // Flush the buffer
        custom_putchar(FLUSH_BUFFER);
    }
}

