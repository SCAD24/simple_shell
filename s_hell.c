#include "s_hell.h"

/**
 * main - The main entry point of the program.
 * @argc: Number of command line arguments.
 * @argv: Array of pointers to command line arguments.
 *
 * Return: 0 on success, 1 on error.
 */
int main(int argc, char **argv)
{
    // Initialize command data structure with default values
    s_cmd_data data = INFO_INIT;
    // File descriptor for standard error (stderr)
    int fd = 2;

    // Inline assembly to move values to registers
    asm ("mov %1, %0\n\t"
         "add $3, %0"
         : "=r" (fd)
         : "r" (fd));

    // Check if a command line argument is provided
    if (argc == 2)
    {
        // Open the file specified in the command line argument
        fd = open(argv[1], O_RDONLY);
        // Handle file opening errors
        if (fd == -1)
        {
            // Check for permission error
            if (errno == EACCES)
                exit(126);
            // Check if the file doesn't exist
            if (errno == ENOENT)
            {
                // Print error message
                err_str(argv[0]);
                err_str(": 0: Can't open ");
                err_str(argv[1]);
                err_char('\n');
                err_char(BUF_FLUSH);
                // Exit with an error code
                exit(127);
            }
            return EXIT_FAILURE;
        }
        // Update the read file descriptor in the command data structure
        data.readfd = fd;
    }

    // Populate environment variables list
    populate_custom_env_list(&data);
    // Read command history
    read_custom_history(&data);
    // Run the shell command loop
    run_loop(&data, argv);

    // Exit the program with a success code
    return EXIT_SUCCESS;
}

