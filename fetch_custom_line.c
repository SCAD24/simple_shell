#include "s_hell.h"

/**
 * input_buffer - Reads and buffers chained commands.
 * @data: Parameter struct.
 * @buf: Address of buffer.
 * @len: Address of len variable.
 *
 * Return: Bytes read.
 */
ssize_t input_buffer(s_cmd_data *data, char **buf, size_t *len)
{
    ssize_t r = 0;
    size_t len_p = 0;

    if (!*len) {
        free(*buf);
        *buf = NULL;
        signal(SIGINT, interrupt_handler);

        #if USE_GETLINE
        r = getline(buf, &len_p, stdin);
        #else
        r = custom_getline(data, buf, &len_p);
        #endif

        if (r > 0) {
            if ((*buf)[r - 1] == '\n') {
                (*buf)[r - 1] = '\0'; // Remove trailing newline.
                r--;
            }
            data->line_count = 1;
            remove_custom_comments(*buf);
            build_custom_history_list(data, *buf, data->hist_count++);

            // Check if the command is a chain.
            {
                *len = r;
                data->cmd_buf = buf;
            }
        }
    }
    return r;
}

/**
 * get_input - Gets a line excluding the newline character.
 * @data: Parameter struct.
 *
 * Return: Bytes read.
 */
ssize_t get_input(s_cmd_data *data)
{
    static char *buf; // The ';' command chain buffer.
    static size_t i, j, len;
    ssize_t r = 0;
    char **buf_p = &(data->arg), *p;

    custom_putchar(BUF_FLUSH);
    r = input_buffer(data, &buf, &len);

    if (r == -1) // EOF
        return -1;

    if (len) { // There are commands left in the chain buffer.
        j = i;
        p = buf + i;

        check_custom_chain(data, buf, &j, i, len);
        
        while (j < len) { // Iterate to semicolon or end.
            if (is_custom_chain(data, buf, &j))
                break;
            j++;
        }

        i = j + 1; // Increment past nulled ';'.

        if (i >= len) { // Reached the end of the buffer?
            i = len = 0; // Reset position and length.
            data->cmd_buf_type = CMD_UNIQUE;
        }

        *buf_p = p; // Pass back pointer to the current command position.
        return custom_strlen(p); // Return length of the current command.
    }

    *buf_p = buf; // Not a chain, pass back buffer from custom_getline().
    return r; // Return length of buffer from custom_getline().
}

/**
 * read_buffer - Reads a buffer.
 * @data: Parameter struct.
 * @buf: Buffer.
 * @i: Size.
 *
 * Return: r.
 */
ssize_t read_buffer(s_cmd_data *data, char *buf, size_t *i)
{
    ssize_t r = 0;

    if (*i)
        return 0;

    r = read(data->readfd, buf, READ_BUFFER_SIZE);

    if (r >= 0)
        *i = r;

    return r;
}

/**
 * custom_getline - Gets the next line of input from STDIN.
 * @data: Parameter struct.
 * @ptr: Address of pointer to buffer, preallocated or NULL.
 * @length: Size of preallocated ptr buffer if not NULL.
 *
 * Return: s.
 */
int custom_getline(s_cmd_data *data, char **ptr, size_t *length)
{
    static char buf[READ_BUFFER_SIZE];
    static size_t i, len;
    size_t k;
    ssize_t r = 0, s = 0;
    char *p = NULL, *new_p = NULL, *c;

    p = *ptr;

    if (p && length)
        s = *length;

    if (i == len)
        i = len = 0;

    r = read_buffer(data, buf, &len);

    if (r == -1 || (r == 0 && len == 0))
        return -1;

    c = custom_strchr(buf + i, '\n');
    k = c ? 1 + (unsigned int)(c - buf) : len;
    new_p = custom_realloc(p, s, s ? s + k : k + 1);

    if (!new_p) // MALLOC FAILURE!
        return p ? free(p), -1 : -1;

    if (s)
        custom_strncat(new_p, buf + i, k - i);
    else
        copy_str(new_p, buf + i, k - i + 1);

    s += k - i;
    i = k;
    p = new_p;

    if (length)
        *length = s;

    *ptr = p;
    return s;
}

/**
 * interrupt_handler - Blocks Ctrl-C.
 * @sig_num: The signal number.
 *
 * Return: Void.
 */
void interrupt_handler(__attribute__((unused))int sig_num)
{
    custom_puts("\n");
    custom_puts("$ ");
    custom_putchar(FLUSH_BUFFER);
}

