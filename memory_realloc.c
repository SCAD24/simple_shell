#include "s_hell.h"

/**
 * custom_memset - Fills memory with a constant byte.
 * @s: The pointer to the memory area.
 * @b: The byte to fill *s with.
 * @n: The number of bytes to be filled.
 *
 * Returns: A pointer to the memory area s.
 */
char *custom_memset(char *s, char b, unsigned int n)
{
    // Loop through each byte and set it to the specified value 'b'.
    for (unsigned int i = 0; i < n; i++)
        s[i] = b;

    return s;
}

/**
 * custom_free_strings - Frees a list of strings.
 * @pp: List of strings.
 */
void custom_free_strings(char **pp)
{
    // Check if the list exists.
    if (!pp)
        return;

    // Loop through the list and free each string.
    while (*pp)
        free(*pp++);

    // Free the list itself.
    free(pp);
}

/**
 * custom_realloc - Reallocates a block of memory.
 * @ptr: Pointer to the previous allocated block.
 * @old_size: Size of the previous block in bytes.
 * @new_size: Size of the new block in bytes.
 *
 * Returns: Pointer to the newly allocated block.
 */
void *custom_realloc(void *ptr, unsigned int old_size, unsigned int new_size)
{
    char *p;

    // If the original block is empty, allocate a new block.
    if (!ptr)
        return malloc(new_size);

    // If the new size is zero, free the original block and return NULL.
    if (!new_size)
        return (free(ptr), NULL);

    // If the new size is the same as the old size, return the original block.
    if (new_size == old_size)
        return ptr;

    // Allocate a new block with the specified size.
    p = malloc(new_size);

    // Check if the allocation was successful.
    if (!p)
        return NULL;

    // Copy data from the old block to the new block.
    old_size = old_size < new_size ? old_size : new_size;
    while (old_size--)
        p[old_size] = ((char *)ptr)[old_size];

    // Free the old block.
    free(ptr);

    // Return the pointer to the newly allocated block.
    return p;
}

