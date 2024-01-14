#include "s_hell.h"

/**
 * check_interactive_mode - Checks if the current process is interacting with a terminal.
 * @data: A pointer to the cmd_data struct holding information about the command being executed.
 *
 * Return: 1 if in interactive mode, 0 otherwise.
 */
int check_interactive_mode(s_cmd_data *data)
{
    return (isatty(STDIN_FILENO) && data->readfd <= 2);
}

/**
 * is_delimiter - Checks if a character is a delimiter.
 * @c: The character to be checked.
 * @delimiters: String of delimiters to compare with.
 *
 * Return: 1 if true, 0 if false.
 */
int is_delimiter(char c, char *delimiters)
{
    while (*delimiters) {
        if (*delimiters++ == c)
            return 1;
    }
    return 0;
}

/**
 * is_alpha - Checks if the input character is an alphabet character.
 * @k: The character to be checked.
 *
 * Return: 1 if the character is an alphabet character, 0 otherwise.
 */
int is_alpha(int k)
{
    if ((k >= 'a' && k <= 'z') || (k >= 'A' && k <= 'Z'))
        return 1;
    else
        return 0;
}

/**
 * custom_atoi - Converts a string to an integer.
 * @s: The string to be converted.
 *
 * Return: 0 if no numbers in string, converted number otherwise.
 */
int custom_atoi(char *s)
{
    int index, sign = 1, flag = 0, output;
    unsigned int result = 0;

    for (index = 0; s[index] != '\0' && flag != 2; index++) {
        if (s[index] == '-')
            sign *= -1;

        if (s[index] >= '0' && s[index] <= '9') {
            flag = 1;
            result *= 10;
            result += (s[index] - '0');
        } else if (flag == 1)
            flag = 2;
    }

    if (sign == -1)
        output = -result;
    else
        output = result;

    return output;
}

