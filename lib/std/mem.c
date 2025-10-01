#include "bruter.h"

void feraw_malloc(BruterList* stack)
{
    BruterInt size = bruter_pop_int(stack);
    void *ptr = malloc(size);
    if (ptr == NULL)
    {
        fprintf(stderr, "ERROR: Memory allocation failed for size %" PRIdPTR "\n", size);
        exit(EXIT_FAILURE);
    }
    bruter_push_pointer(stack, ptr, NULL, BRUTER_TYPE_BUFFER);
}

void feraw_calloc(BruterList* stack)
{
    BruterInt count = bruter_pop_int(stack);
    BruterInt size = bruter_pop_int(stack);
    void *ptr = calloc(count, size);
    if (ptr == NULL)
    {
        fprintf(stderr, "ERROR: Memory allocation failed for count %" PRIdPTR " and size %" PRIdPTR "\n", count, size);
        exit(EXIT_FAILURE);
    }
    bruter_push_pointer(stack, ptr, NULL, BRUTER_TYPE_BUFFER);
}

void feraw_realloc(BruterList* stack)
{
    void *ptr = bruter_pop_pointer(stack);
    BruterInt new_size = bruter_pop_int(stack);
    void *new_ptr = realloc(ptr, new_size);
    if (new_ptr == NULL)
    {
        fprintf(stderr, "ERROR: Memory reallocation failed for size %" PRIdPTR "\n", new_size);
        exit(EXIT_FAILURE);
    }
    bruter_push_pointer(stack, new_ptr, NULL, BRUTER_TYPE_BUFFER);
}

void feraw_memset(BruterList* stack)
{
    void *ptr = bruter_pop_pointer(stack);
    BruterInt value = bruter_pop_int(stack);
    BruterInt size = bruter_pop_int(stack);
    
    if (ptr == NULL)
    {
        fprintf(stderr, "ERROR: Cannot set memory, pointer is NULL\n");
        exit(EXIT_FAILURE);
    }
    
    memset(ptr, value, size);
}

void feraw_memcpy(BruterList* stack)
{
    void *dest = bruter_pop_pointer(stack);
    void *src = bruter_pop_pointer(stack);
    BruterInt size = bruter_pop_int(stack);
    
    if (dest == NULL || src == NULL)
    {
        fprintf(stderr, "ERROR: Cannot copy memory, one of the pointers is NULL\n");
        exit(EXIT_FAILURE);
    }
    
    memcpy(dest, src, size);
}

void feraw_memcmp(BruterList* stack)
{
    void *ptr1 = bruter_pop_pointer(stack);
    void *ptr2 = bruter_pop_pointer(stack);
    BruterInt size = bruter_pop_int(stack);
    
    if (ptr1 == NULL || ptr2 == NULL)
    {
        fprintf(stderr, "ERROR: Cannot compare memory, one of the pointers is NULL\n");
        exit(EXIT_FAILURE);
    }
    
    BruterInt result = memcmp(ptr1, ptr2, size);
    bruter_push_int(stack, result, NULL, BRUTER_TYPE_ANY);
}

void feraw_memmove(BruterList* stack)
{
    void *dest = bruter_pop_pointer(stack);
    void *src = bruter_pop_pointer(stack);
    BruterInt size = bruter_pop_int(stack);
    
    if (dest == NULL || src == NULL)
    {
        fprintf(stderr, "ERROR: Cannot move memory, one of the pointers is NULL\n");
        exit(EXIT_FAILURE);
    }
    
    memmove(dest, src, size);
}