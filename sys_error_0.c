#include "s_hell.h"

/**
 * custom_str - Prints characters in the given string to the standard error output.
 * @str: The string to print.
 *
 * Return: Nothing
 */
void custom_str(char *str)
{
    int i = 0;

    if (!str)
        return;
    while (str[i] != '\0')
    {
        custom_char(str[i]);
        i++;
    }
}

/**
 * custom_char - Writes a character to the standard error output.
 * @c: The character to be written.
 *
 * Return: Returns 1 on success, otherwise returns an error code.
 */
int custom_char(char c)
{
    static int i;
    static char buf[WRITE_BUFFER_SIZE];

    if (c == FLUSH_BUFFER || i >= WRITE_BUFFER_SIZE)
    {
        write(2, buf, i);
        i = 0;
    }
    if (c != FLUSH_BUFFER)
        buf[i++] = c;
    return (1);
}

/**
 * custom_fd - Writes a character to a file descriptor buffer.
 * @c: The character to be written.
 * @fd: The file descriptor to write to.
 *
 * Return: On success, returns 1.
 * On error, returns -1 and sets errno appropriately.
 */
int custom_fd(char c, int fd)
{
    static int z;
    static char buf[WRITE_BUFFER_SIZE];

    if (c == FLUSH_BUFFER || z >= WRITE_BUFFER_SIZE)
    {
        write(fd, buf, z);
        z = 0;
    }
    if (c != FLUSH_BUFFER)
        buf[z++] = c;
    return (1);
}

/**
 * custom_puts - Writes a string to a file descriptor.
 * @str: The string to be written.
 * @fd: The file descriptor to write to.
 *
 * Return: The number of characters written, or 0 if the input string is NULL.
 */
int custom_puts(char *str, int fd)
{
    int b = 0;

    if (!str)
        return (0);
    while (*str)
    {
        b += custom_fd(*str++, fd);
    }
    return (b);
}

