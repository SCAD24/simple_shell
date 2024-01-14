#include "s_hell.h"

/*
 * File: sys_error_1.c
 * Auth: Simon Chao
 */

/**
 * custom_error_atoi - Converts a string representation of an integer to an integer value.
 * @s: The string representation of the integer to be converted.
 * Return: The integer value of the input string, or -1 if the string is invalid.
 */
int custom_error_atoi(char *s)
{
    int i = 0;
    unsigned long int result = 0;

    if (*s == '+')
        s++;
    for (i = 0; s[i] != '\0'; i++)
    {
        if (s[i] >= '0' && s[i] <= '9')
        {
            result *= 10;
            result += (s[i] - '0');
            if (result > INT_MAX)
                return (-1);
        }
        else
            return (-1);
    }
    return (result);
}

/**
 * display_custom_error - Print an error message to the standard error stream.
 * @data: A pointer to s_cmd_data representing the command data.
 * @estr: A pointer to a char array representing the error message to be printed.
 * Return: 0 if no numbers in string, converted number otherwise, -1 on error.
 */
void display_custom_error(s_cmd_data *data, char *estr)
{
    custom_str(data->filename);
    custom_str(": ");
    print_digits(data->index, STDERR_FILENO);
    custom_str(": ");
    custom_str(data->argv[0]);
    custom_str(": ");
    custom_str(estr);
}

/*
 * print_digits - Function prints a decimal (integer) number (base 10).
 * @input: The input integer.
 * @fd: The file descriptor to write to.
 *
 * Return: Number of characters printed.
 */
int print_digits(int input, int fd)
{
    int (*custom_putc)(char, int) = custom_putchar;
    int i, count = 0;
    unsigned int _abs_, current;

    if (fd == STDERR_FILENO)
        custom_putc = custom_fd;
    if (input < 0)
    {
        _abs_ = -input;
        custom_putc('-', fd);
        count++;
    }
    else
        _abs_ = input;
    current = _abs_;
    for (i = 1000000000; i > 1; i /= 10)
    {
        if (_abs_ / i)
        {
            custom_putc('0' + current / i, fd);
            count++;
        }
        current %= i;
    }
    custom_putc('0' + current, fd);
    count++;

    return (count);
}

/**
 * custom_convert_number - Converter function, a clone of itoa.
 * @num: The number.
 * @base: The base.
 * @flags: Argument flags.
 *
 * Return: String.
 */
char *custom_convert_number(long int num, int base, int flags)
{
    static char *array;
    static char buffer[50];
    char sign = 0;
    char *ptr;
    unsigned long n = num;

    if (!(flags & CONV_UNSIGNED) && num < 0)
    {
        n = -num;
        sign = '-';
    }
    array = flags & CONV_LOWER ? "0123456789abcdef" : "0123456789ABCDEF";
    ptr = &buffer[49];
    *ptr = '\0';

    do
    {
        *--ptr = array[n % base];
        n /= base;
    } while (n != 0);

    if (sign)
        *--ptr = sign;
    return (ptr);
}

/**
 * remove_custom_comments - Function replaces first instance of '#' with '\0'.
 * @buf: Address of the string to modify.
 *
 * Return: Always 0.
 */
void remove_custom_comments(char *buf)
{
    int i;

    for (i = 0; buf[i] != '\0'; i++)
        if (buf[i] == '#' && (!i || buf[i - 1] == ' '))
        {
            buf[i] = '\0';
            break;
        }
}

