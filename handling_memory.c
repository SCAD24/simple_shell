#include "s_hell.h"

/**
 * custom_dealloc - Frees the memory pointed to by a pointer and sets it to NULL.
 * @ptr: A pointer to a pointer to the memory to be freed.
 *
 * Returns: 1 if memory was successfully freed, 0 otherwise.
 */
int custom_dealloc(void **ptr)
{
    // Check if the pointer and the memory it points to exist
    if (ptr && *ptr)
    {
        // Free the memory and set the pointer to NULL
        free(*ptr);
        *ptr = NULL;

        // Memory freed successfully
        return 1;
    }

    // Memory was not freed
    return 0;
}

