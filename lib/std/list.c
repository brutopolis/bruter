#include "bruter.h"

// functions
// functions
// functions
// functions

void feraw_list(BruterList* stack)
{
    BruterInt size = bruter_pop_int(stack);
    BruterList *list = bruter_new(BRUTER_DEFAULT_SIZE, true, true);
    for (BruterInt i = 0; i < size; i++)
    {
        BruterMeta value = bruter_pop_meta(stack);
        bruter_push_meta(list, value);
    }
    bruter_push_pointer(stack, list, NULL, BRUTER_TYPE_LIST);
}

void feraw_pop(BruterList* stack)
{
    BruterList* list = bruter_pop_pointer(stack);
    BruterMeta value = bruter_pop_meta(list);
    free(value.key); // Free the key if it exists
    bruter_push_meta(stack, value); // Push the popped value back to the stack
}

void feraw_push(BruterList* stack)
{
    BruterList* list = bruter_pop_pointer(stack);
    BruterMeta value = bruter_pop_meta(stack);
    bruter_push_meta(list, value);
}

void feraw_shift(BruterList* stack)
{
    BruterList* list = bruter_pop_pointer(stack);
    BruterMeta value = bruter_shift_meta(list);
    free(value.key); // Free the key if it exists
    bruter_push_meta(stack, value); // Push the shifted value back to the stack
}

void feraw_unshift(BruterList* stack)
{
    BruterList* list = bruter_pop_pointer(stack);
    BruterMeta value = bruter_pop_meta(stack);
    bruter_unshift_meta(list, value);
}

void feraw_insert(BruterList* stack)
{
    BruterList* list = bruter_pop_pointer(stack);
    BruterInt index = bruter_pop_int(stack);
    BruterMeta value = bruter_pop_meta(stack);

    if (index < 0) // -1 = the last element and so on
    {
        index += list->size; // Adjust negative index to positive
    }

    bruter_insert_meta(list, index, value);
}

void feraw_remove(BruterList* stack)
{
    BruterList* list = bruter_pop_pointer(stack);
    BruterInt index = bruter_pop_int(stack);

    if (index < 0) // -1 = the last element and so on
    {
        index += list->size; // Adjust negative index to positive
    }

    BruterMeta removed_value = bruter_remove_meta(list, index);
    free(removed_value.key); // Free the key if it exists
    bruter_push_meta(stack, removed_value); // Push the removed value back to the stack
}

void feraw_get(BruterList* stack)
{
    BruterMeta list_meta = bruter_pop_meta(stack);
    BruterMeta index_meta = bruter_pop_meta(stack);
    BruterInt index = index_meta.value.i;
    
    if (list_meta.type == BRUTER_TYPE_BUFFER)
    {
        char* buffer = (char*)list_meta.value.p;
        if (index_meta.type == BRUTER_TYPE_FLOAT)
        {
            index = (BruterInt)index_meta.value.f; // Convert float to integer if necessary
        }
        
        if (index < 0) // -1 = the last element and so on
        {
            index += strlen(buffer); // Adjust negative index to positive
        }

        bruter_push_int(stack, buffer[index], NULL, BRUTER_TYPE_ANY);
        return;
    }
    else
    {
        BruterList* list = (BruterList*)list_meta.value.p;
        if (index_meta.type == BRUTER_TYPE_FLOAT)
        {
            index = (BruterInt)index_meta.value.f; // Convert float to integer if necessary
        }
        else if (index_meta.type == BRUTER_TYPE_BUFFER)
        {
            index = bruter_find_key(list_meta.value.p, (char*)index_meta.value.p);
            if (index < 0)
            {
                fprintf(stderr, "ERROR: cant get, key '%s' not found in list\n", (char*)index_meta.value.p);
                exit(EXIT_FAILURE);
            }
        }

        if (index < 0) // -1 = the last element and so on
        {
            index += list->size; // Adjust negative index to positive
        }

        bruter_push_meta(stack, bruter_get_meta(list, index));
    }
}

void feraw_set(BruterList* stack)
{
    BruterMeta list_meta = bruter_pop_meta(stack);
    BruterMeta index = bruter_pop_meta(stack);
    BruterMeta value = bruter_pop_meta(stack);
    if (list_meta.type == BRUTER_TYPE_BUFFER)
    {
        unsigned char* buffer = (unsigned char*)list_meta.value.p;
        if (index.type == BRUTER_TYPE_FLOAT)
        {
            index.value.i = (BruterInt)index.value.f; // Convert float to integer if necessary
        }
        
        if (index.value.i < 0) // -1 = the last element and so on
        {
            index.value.i += strlen((char*)buffer); // Adjust negative index to positive
        }

        buffer[index.value.i] = value.value.i; // Directly set the value in the buffer
    }
    else
    {
        BruterList* list = (BruterList*)list_meta.value.p;
        switch (index.type)
        {
            case BRUTER_TYPE_FLOAT:
                index.value.i = (BruterInt)index.value.f;
            case BRUTER_TYPE_ANY:
                if (index.value.i >= list->size)
                {
                    while (list->size <= index.value.i)
                    {
                        bruter_push_meta(list, (BruterMeta){.value = {.i = 0}, .key = NULL, .type = BRUTER_TYPE_ANY});
                    }
                }
                else if (index.value.i < 0) // -1 = the last element and so on
                {
                    index.value.i += list->size; // Adjust negative index to positive
                }
                list->data[index.value.i] = value.value; // Directly set the value
                if (list->types != NULL)
                {
                    list->types[index.value.i] = value.type; // Set the type   
                }
                break;
            case BRUTER_TYPE_BUFFER:
            {
                BruterInt found_index = bruter_find_key(list, (char*)index.value.p);
                if (found_index < 0)
                {
                    value.key = strdup(index.value.p);
                    bruter_push_meta(list, value); // Push the value to the list
                }
                else 
                {
                    list->data[found_index] = value.value; // Directly set the value
                    if (list->keys != NULL)
                    {
                        if (list->keys[found_index] == NULL)
                        {
                            list->keys[found_index] = index.value.p; // Set the new key
                        }
                        else
                        {
                            free(value.key); // Free the old key if it was allocated
                        }
                    }
                    if (list->types != NULL)
                    {
                        list->types[found_index] = value.type; // Set the type   
                    }
                }
            }
            break;
        }
        
    }
}

void feraw_where(BruterList* stack)
{
    BruterList* list = bruter_pop_pointer(stack);
    BruterMeta value = bruter_pop_meta(stack);
    BruterInt found_index = -1;

    for (BruterInt i = 0; i < list->size; i++)
    {
        if (list->types[i] == value.type && list->data[i].i == value.value.i)
        {
            found_index = i;
            break;
        }
    }

    bruter_push_int(stack, found_index, NULL, BRUTER_TYPE_ANY);
}

void feraw_find(BruterList* stack)
{
    BruterList* list = bruter_pop_pointer(stack);
    char* key = bruter_pop_pointer(stack);
    BruterInt found_index = -1;

    for (BruterInt i = 0; i < list->size; i++)
    {
        if (list->keys[i] != NULL && strcmp(list->keys[i], key) == 0)
        {
            found_index = i;
            break;
        }
    }

    bruter_push_int(stack, found_index, NULL, BRUTER_TYPE_ANY);
}

void feraw_length(BruterList* stack)
{
    BruterList* list = bruter_pop_pointer(stack);
    bruter_push_int(stack, list->size, NULL, BRUTER_TYPE_ANY);
}

void feraw_copy(BruterList* stack)
{
    BruterList* list = bruter_pop_pointer(stack);
    BruterList* new_list = bruter_copy(list);
}

void feraw_swap(BruterList* stack)
{
    BruterList* list = bruter_pop_pointer(stack);
    BruterInt index1 = bruter_pop_int(stack);
    BruterInt index2 = bruter_pop_int(stack);

    if (index1 < 0 || index1 >= list->size || index2 < 0 || index2 >= list->size)
    {
        fprintf(stderr, "ERROR: cannot swap, index %" PRIdPTR " or %" PRIdPTR " out of range in list of size %" PRIdPTR "\n", index1, index2, list->size);
        exit(EXIT_FAILURE);
    }

    bruter_swap(list, index1, index2);
}

void feraw_reverse(BruterList* stack)
{
    BruterList* list = bruter_pop_pointer(stack);
    bruter_reverse(list);
}

void feraw_point(BruterList* stack)
{
    BruterList* list = bruter_pop_pointer(stack);
    BruterMeta value = bruter_pop_meta(stack);
    if (value.type == BRUTER_TYPE_BUFFER) // we assume its the key of the element we wanto to point to
    {
        BruterInt index = bruter_find_key(list, (char*)value.value.p);
        if (index < 0)
        {
            fprintf(stderr, "ERROR: cannot point to, key '%s' not found in list\n", (char*)value.value.p);
            exit(EXIT_FAILURE);
        }
        bruter_push_pointer(stack, &list->data[index], NULL, BRUTER_TYPE_BUFFER);
    }
    else // we assume its the index of the element we want to point to
    {
        if (value.value.i < 0) // -1 = the last element and so on
        {
            value.value.i += list->size; // Adjust negative index to positive
        }
        if (value.value.i < 0 || value.value.i >= list->size)
        {
            fprintf(stderr, "ERROR: cannot point to, index %" PRIdPTR " out of range in list of size %" PRIdPTR "\n", value.value.i, list->size);
            exit(EXIT_FAILURE);
        }
        bruter_push_pointer(stack, &list->data[value.value.i], NULL, BRUTER_TYPE_BUFFER);
    }
}

void feraw_alloc(BruterList* stack)
{
    BruterList *arena = bruter_pop_pointer(stack);
    BruterInt size = bruter_pop_int(stack);
    void *ptr = bruter_alloc(arena, size);
    bruter_push_pointer(stack, ptr, NULL, BRUTER_TYPE_BUFFER);
}

void feraw_dup(BruterList* stack)
{
    BruterMeta value = bruter_pop_meta(stack);
    bruter_push_meta(stack, value); // Push the value back to the stack
    bruter_push_meta(stack, value); // Duplicate it
}

void feraw_free(BruterList* stack)
{
    BruterMeta value = bruter_pop_meta(stack);
    if (value.type == BRUTER_TYPE_BUFFER)
    {
        free(value.value.p); // Free the buffer if it was allocated
    }
    else
    {
        bruter_free((BruterList*)value.value.p); // Free the list if it was allocated
    }
    // No action needed for other types, as they are not dynamically allocated
}

void feraw_nameof(BruterList* stack)
{
    BruterList* list = bruter_pop_pointer(stack);
    BruterMeta index_meta = bruter_pop_meta(stack);
    switch (index_meta.type)
    {
        case BRUTER_TYPE_FLOAT:
            index_meta.value.i = (BruterInt)index_meta.value.f; // Convert float to integer if necessary
            break;
        case BRUTER_TYPE_ANY:
            // No conversion needed
            break;
        case BRUTER_TYPE_BUFFER:
            index_meta.value.i = bruter_find_key(list, (char*)index_meta.value.p);
            if (index_meta.value.i < 0)
            {
                fprintf(stderr, "ERROR: key '%s' not found in list\n", (char*)index_meta.value.p);
                exit(EXIT_FAILURE);
            }
            break;
        default:
            fprintf(stderr, "ERROR: key expects a float or any type, got %d\n", index_meta.type);
            exit(EXIT_FAILURE);
    }

    if (index_meta.value.i < 0)
    {
        index_meta.value.i += list->size; // Adjust negative index to positive
    }

    if (index_meta.value.i < 0 || index_meta.value.i >= list->size)
    {
        fprintf(stderr, "ERROR: key index %" PRIdPTR " out of range in list of size %" PRIdPTR "\n", index_meta.value.i, list->size);
        exit(EXIT_FAILURE);
    }

    bruter_push_meta(stack, (BruterMeta){.value = {.p = list->keys[index_meta.value.i]}, .key = NULL, .type = BRUTER_TYPE_BUFFER});
}

void feraw_typeof(BruterList* stack)
{
    BruterList* list = bruter_pop_pointer(stack);
    BruterMeta index = bruter_pop_meta(stack);
    if (index.type == BRUTER_TYPE_FLOAT)
    {
        index.value.i = (BruterInt)index.value.f; // Convert float to integer if necessary
    }
    else if (index.type == BRUTER_TYPE_BUFFER)
    {
        index.value.i = bruter_find_key(list, (char*)index.value.p);
        if (index.value.i < 0)
        {
            fprintf(stderr, "ERROR: cannot typeof, key '%s' not found in list\n", (char*)index.value.p);
            exit(EXIT_FAILURE);
        }
    }

    if (index.value.i < 0 || index.value.i >= list->size)
    {
        fprintf(stderr, "ERROR: cannot typeof, index %" PRIdPTR " out of range in list of size %" PRIdPTR "\n", index.value.i, list->size);
        exit(EXIT_FAILURE);
    }
    bruter_push_int(stack, list->types[index.value.i], NULL, BRUTER_TYPE_ANY);
}

void feraw_rename(BruterList* stack)
{
    BruterList* list = bruter_pop_pointer(stack);
    BruterMeta index = bruter_pop_meta(stack);
    char* newKey = bruter_pop_pointer(stack);
    if (index.type == BRUTER_TYPE_FLOAT)
    {
        index.value.i = (BruterInt)index.value.f; // Convert float to integer if necessary
    }
    else if (index.type == BRUTER_TYPE_BUFFER)
    {
        index.value.i = bruter_find_key(list, (char*)index.value.p);
        if (index.value.i < 0)
        {
            fprintf(stderr, "ERROR: cannot rename, key '%s' not found in list\n", (char*)index.value.p);
            exit(EXIT_FAILURE);
        }
    }
    if (index.value.i < 0 || index.value.i >= list->size)
    {
        fprintf(stderr, "ERROR: cannot rename, index %" PRIdPTR " out of range in list of size %" PRIdPTR "\n", index.value.i, list->size);
        exit(EXIT_FAILURE);
    }
    if (list->keys == NULL)
    {
        list->keys = calloc(list->capacity, sizeof(char*));
    }
    if (list->keys[index.value.i] != NULL)
    {
        free(list->keys[index.value.i]); // Free the old key if it was allocated
    }
    list->keys[index.value.i] = strdup(newKey); // Set the new key
}

void feraw_retype(BruterList* stack)
{
    BruterList* list = bruter_pop_pointer(stack);
    BruterMeta index = bruter_pop_meta(stack);
    BruterInt newType = bruter_pop_int(stack);
    if (index.type == BRUTER_TYPE_FLOAT)
    {
        index.value.i = (BruterInt)index.value.f; // Convert float to integer if necessary
    }
    else if (index.type == BRUTER_TYPE_BUFFER)
    {
        index.value.i = bruter_find_key(list, (char*)index.value.p);
        if (index.value.i < 0)
        {
            fprintf(stderr, "ERROR: cannot retype, key '%s' not found in list\n", (char*)index.value.p);
            exit(EXIT_FAILURE);
        }
    }

    if (index.value.i < 0 || index.value.i >= list->size)
    {
        fprintf(stderr, "ERROR: cannot retype, index %" PRIdPTR " out of range in list of size %" PRIdPTR "\n", index.value.i, list->size);
        exit(EXIT_FAILURE);
    }
    list->types[index.value.i] = newType;
}