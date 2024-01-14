#ifndef S_HELL_H
#define S_HELL_H

/*
 * File: s_hell.h
 * Auth: Simon Chao
 * Description: Header file containing function prototypes and data structures
 *              for a simple shell program. It includes macros, constants,
 *              and function prototypes.
 */

/* Standard Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <limits.h>
#include <errno.h>

/* Constants for command chaining */
#define CMD_UNIQUE     0
#define CMD_DIFFERENT  1
#define CMD_VARIED     2
#define CMD_REPLACED   3

/* Constants for number conversion */
#define CONV_LOWER     1
#define CONV_UNSIGNED  2

/* Buffer sizes */
#define READ_BUFFER_SIZE  1024
#define WRITE_BUFFER_SIZE 1024
#define FLUSH_BUFFER      -1

#define HISTORY_FILE    ".simple_shell_history"
#define MAX_HISTORY     4096

extern char **environ;

/* Struct for a linked list */
typedef struct liststr {
    int num;
    char *str;
    struct liststr *next;
} s_list;

/* Struct for command line input data */
typedef struct cmd_data_struct {
    char *arg;
    char **argv;
    char *course;
    int arg_count;
    unsigned int index;
    int xcode;
    int line_count;
    char *filename;
    s_list *tsh_env;
    s_list *cmd_log;
    s_list *tsh_alias;
    char **environ;
    int env_changed;
    int tsh_status;
    char **cmd_buf;
    int cmd_buf_type;
    int readfd;
    int hist_count;
} s_cmd_data;

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
    0, 0, 0}

/* Struct for a built-in command and its associated function */
typedef struct registry {
    char *type;
    int (*func)(s_cmd_data *);
} s_builtin_reg;

/* function placeholders */
int unique_run(s_cmd_data *, char **);
int different_search(s_cmd_data *);
void varied_path(s_cmd_data *);
void replaced_fork(s_cmd_data *);

/* function placeholders */
int unique_run(s_cmd_data *, char **);
int different_search(s_cmd_data *);
void varied_path(s_cmd_data *);
void replaced_fork(s_cmd_data *);

/* Processor functions */
int distinct_exe(s_cmd_data *, char *);
char *duplicate_chars(char *, int, int);
char *absolute_route(s_cmd_data *, char *, char *);

/* Handle system errors */
void custom_str(char *);
int custom_char(char);
int custom_fd(char c, int fd);
int custom_puts(char *str, int fd);

/* Utility string functions */
int custom_strlen(char *);
int custom_strcmp(char *, char *);
char *match_prefix(const char *, const char *);
char *custom_strcat(char *, char *);

/* Other string operations */
char *custom_strcpy(char *, char *);
char *custom_strdup(const char *);
void custom_puts(char *);
int custom_putchar(char);

/* Handle strings */
char *copy_str(char *, char *, int);
char *custom_strncat(char *, char *, int);
char *custom_strchr(char *, char);

/* Tokenizer functions */
char **unique_tokens(char *, char *);
char **unique_tokens2(char *, char);

/* Memory reallocation */
char *custom_memset(char *, char, unsigned int);
void free_custom(char **);
void *custom_realloc(void *, unsigned int, unsigned int);

/* Memory handling */
int custom_dealloc(void **);

/* Number conversion functions */
int interact_shell(s_cmd_data *);
int tokenize_delimiter(char, char *);
int check_alpha(int);
int custom_atoi(char *);

/* System error handling */
int custom_error_atoi(char *);
void display_custom_error(s_cmd_data *, char *);
int print_digits(int, int);
char *custom_convert_number(long int, int, int);
void remove_custom_comments(char *);

/* Built-in commands */
int exit_shell(s_cmd_data *);
int change_directory(s_cmd_data *);
int help_cd(s_cmd_data *);

/* Additional built-in functions */
int shell_history(s_cmd_data *);
int my_custom_alias(s_cmd_data *);

/* Input fetching */
ssize_t fetch_input(s_cmd_data *);
int custom_getline(s_cmd_data *, char **, size_t *);
void interrupt_handler(int);

/* Data handling */
void clear_shell_data(s_cmd_data *);
void setup_shell_data(s_cmd_data *, char **);
void release_shell_data(s_cmd_data *, int);

/* Environment operations */
char *fetch_shell_env(s_cmd_data *, const char *);
int my_custom_env(s_cmd_data *);
int my_custom_setenv(s_cmd_data *);
int my_custom_unsetenv(s_cmd_data *);
int populate_custom_env_list(s_cmd_data *);

/* More environment handling */
char **get_custom_environ(s_cmd_data *);
int unset_custom_env(s_cmd_data *, char *);
int initialize_shell_env(s_cmd_data *, char *, char *);

/* History operations */
char *get_custom_history_file(s_cmd_data *data);
int write_custom_history(s_cmd_data *data);
int read_custom_history(s_cmd_data *data);
int build_custom_history_list(s_cmd_data *data, char *buf, int linecount);
int renumber_custom_history(s_cmd_data *data);

/* Utility linked list functions */
s_list *custom_add_node(s_list **, const char *, int);
s_list *add_custom_node_end(s_list **, const char *, int);
size_t print_custom_list_str(const s_list *);
int delete_custom_node_at_index(s_list **, unsigned int);
void custom_free_list(s_list **);

/* More linked list operations */
size_t custom_list_length(const s_list *);
char **list_to_custom_strings(s_list *);
size_t custom_print_list(const s_list *);
s_list *node_starts_with(s_list *, char *, char);
ssize_t get_custom_node_index(s_list *, s_list *);

/* Shell parameter handling */
int check_custom_chain(s_cmd_data *, char *, size_t *);
void verify_chain(s_cmd_data *, char *, size_t *, size_t, size_t);
int replace_custom_alias(s_cmd_data *);
int replace_custom_vars(s_cmd_data *);
int replace_custom_string(char **, char *);

#endif /* S_HELL_H */
