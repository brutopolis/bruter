#include "bruter.h"

BruterList *bruter_init(BruterInt size, bool is_table)
{
    BruterList *list = (BruterList*)malloc(sizeof(BruterList));
    
    if (list == NULL)
    {
        printf("BRUTER_ERROR: failed to allocate memory for BruterList\n");
        exit(EXIT_FAILURE);
    }
    
    list->data = (size == 0) ? NULL : (BruterValue*)malloc(size * sizeof(BruterValue));
    
    if (size > 0 && list->data == NULL)
    {
        printf("BRUTER_ERROR: failed to allocate memory for BruterList data\n");
        free(list);
        exit(EXIT_FAILURE);
    }
    
    list->size = 0;
    list->capacity = size;

    if (is_table)
    {
        list->keys = (char**)calloc(size, sizeof(char*)); // we need all keys to be NULL
        if (list->keys == NULL)
        {
            printf("BRUTER_ERROR: failed to allocate memory for BruterList keys\n");
            free(list->data);
            free(list);
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        list->keys = NULL;
    }

    return list;
}

void bruter_free(BruterList *list)
{
    free(list->data);
    if (list->keys != NULL)
    {
        for (BruterInt i = 0; i < list->size; i++)
        {
            free(list->keys[i]);
        }
        free(list->keys);
    }
    free(list);
}

void bruter_double(BruterList *list)
{
    list->capacity = list->capacity == 0 ? 1 : list->capacity * 2;
    list->data = realloc(list->data, list->capacity * sizeof(BruterValue));
    if (list->keys != NULL)
    {
        list->keys = realloc(list->keys, list->capacity * sizeof(char*));
        memset(list->keys + list->size, 0, (list->capacity - list->size) * sizeof(char*));
    }
}

void bruter_half(BruterList *list)
{
    list->capacity /= 2;
    list->data = realloc(list->data, list->capacity * sizeof(BruterValue));
    if (list->keys != NULL)
    {
        list->keys = realloc(list->keys, list->capacity * sizeof(char*));
    }

    if (list->size > list->capacity)
    {
        list->size = list->capacity;
    }
}

void bruter_push(BruterList *list, BruterValue value, const char* key)
{
    if (list->size == list->capacity)
    {
        bruter_double(list);
    }
    list->data[list->size] = value;

    if (list->keys != NULL)
    {
        if (key != NULL)
        {
            int len = strlen(key);
            list->keys[list->size] = malloc(len + 1);
            strncpy(list->keys[list->size], key, len);
            list->keys[list->size][len] = '\0';
        }
        else 
        {
            list->keys[list->size] = NULL;
        }
    }
    list->size++;
}

void bruter_unshift(BruterList *list, BruterValue value, const char* key)
{
    if (list->size == list->capacity)
    {
        bruter_double(list);
    }
    memmove(&(list->data[1]), &(list->data[0]), list->size * sizeof(BruterValue));
    list->data[0] = value;
    
    
    if (list->keys != NULL)
    {
        memmove(&(list->keys[1]), &(list->keys[0]), list->size * sizeof(char*));
        if (key != NULL)
        {
            int len = strlen(key);
            list->keys[0] = malloc(len + 1);
            strncpy(list->keys[0], key, len);
            list->keys[0][len] = '\0';
        }
        else 
        {
            list->keys[0] = NULL;
        }
    }
    list->size++;
}

void bruter_insert(BruterList *list, BruterInt i, BruterValue value, const char* key)
{
    if (list->size == list->capacity)
    {
        bruter_double(list);
    }
    if (i <= list->size) 
    {
        memmove(&(list->data[i + 1]), &(list->data[i]), (list->size - i) * sizeof(BruterValue));
        list->data[i] = value;
        if (list->keys != NULL)
        {
            memmove(&(list->keys[i + 1]), &(list->keys[i]), (list->size - i) * sizeof(char*));
            if (key != NULL)
            {
                int len = strlen(key);
                list->keys[i] = malloc(len + 1);
                strncpy(list->keys[i], key, len);
                list->keys[i][len] = '\0';
            }
            else 
            {
                list->keys[i] = NULL;
            }
        }
        list->size++;
    } 
    else 
    {
        printf("BRUTER_ERROR: index %" PRIdPTR " out of range in list of size %" PRIdPTR " \n", i, list->size);
        exit(EXIT_FAILURE);
    }
}

BruterValue bruter_pop(BruterList *list)
{
    if (list->keys != NULL)
    {
        free(list->keys[list->size - 1]);
        list->keys[list->size - 1] = NULL;
    }
    return list->data[--list->size];
}

BruterValue bruter_shift(BruterList *list)
{
    BruterValue ret = list->data[0];
    if (list->size > 1) 
    {
        memmove(&(list->data[0]), &(list->data[1]), (list->size - 1) * sizeof(BruterValue));
    }
    if (list->keys != NULL)
    {
        free(list->keys[0]);
        list->keys[0] = NULL;
        if (list->size > 1) 
        {
            memmove(&(list->keys[0]), &(list->keys[1]), (list->size - 1) * sizeof(char*));
        }
    }
    list->size--;
    return ret;
}

BruterValue bruter_remove(BruterList *list, BruterInt i)
{
    BruterValue ret = list->data[i];
    memmove(&(list->data[i]), &(list->data[i + 1]), (list->size - (i) - 1) * sizeof(BruterValue));
    list->size--;
    if (list->keys != NULL)
    {
        free(list->keys[i]);
        list->keys[i] = NULL;
        memmove(&(list->keys[i]), &(list->keys[i + 1]), (list->size - (i) - 1) * sizeof(char*));
    }
    return ret;
}

BruterValue bruter_fast_remove(BruterList *list, BruterInt i)
{
    BruterValue ret = list->data[i];
    bruter_swap(list, i, list->size - 1);
    bruter_pop(list);
    return ret;
}

void bruter_swap(BruterList *list, BruterInt i1, BruterInt i2)
{
    BruterValue tmp = list->data[i1];
    list->data[i1] = list->data[i2];
    list->data[i2] = tmp;

    if (list->keys != NULL)
    {
        char* tmp_key = list->keys[i1];
        list->keys[i1] = list->keys[i2];
        list->keys[i2] = tmp_key;
    }
}

BruterInt bruter_find(BruterList *list, BruterValue value, const char* key)
{
    if (list->keys != NULL && key != NULL)
    {
        for (BruterInt i = 0; i < list->size; i++)
        {
            if (list->keys[i] && strcmp(list->keys[i], key) == 0)
            {
                return i;
            }
        }
    }
    else if (list->keys == NULL && key != NULL)
    {
        printf("BRUTER_ERROR: bruter_find received a key but the list is not a table\n");
        return -1;
    }
    else
    {
        for (BruterInt i = 0; i < list->size; i++)
        {
            if (list->data[i].i == value.i)
            {
                return i;
            }
        }
    }
    return -1;
}

void bruter_reverse(BruterList *list)
{
    for (BruterInt i = 0; i < list->size / 2; i++)
    {
        bruter_swap(list, i, list->size - i - 1);
    }
}

BruterList* bruter_copy(BruterList *list)
{
    BruterList *copy = bruter_init(list->capacity, list->keys != NULL);
    if (copy == NULL)
    {
        printf("BRUTER_ERROR: failed to allocate memory for BruterList copy\n");
        exit(EXIT_FAILURE);
    }
    copy->size = list->size;
    copy->data = (BruterValue*)malloc(copy->capacity * sizeof(BruterValue));
    if (copy->data == NULL)
    {
        printf("BRUTER_ERROR: failed to allocate memory for BruterList copy data\n");
        free(copy);
        exit(EXIT_FAILURE);
    }
    memcpy(copy->data, list->data, copy->size * sizeof(BruterValue));
    if (list->keys != NULL)
    {
        copy->keys = (char**)malloc(copy->capacity * sizeof(char*));
        if (copy->keys == NULL)
        {
            printf("BRUTER_ERROR: failed to allocate memory for BruterList copy keys\n");
            free(copy->data);
            free(copy);
            exit(EXIT_FAILURE);
        }
        for (BruterInt i = 0; i < copy->size; i++)
        {
            if (list->keys[i] != NULL)
            {
                int len = strlen(list->keys[i]);
                copy->keys[i] = malloc(len + 1);
                strncpy(copy->keys[i], list->keys[i], len);
                copy->keys[i][len] = '\0';
            }
            else
            {
                copy->keys[i] = NULL;
            }
        }
    }
    else
    {
        copy->keys = NULL;
    }
    return copy;
}

// pass NULL for context if you want to call a function directly
// if context exist, the return will be always an int, because it return the index of the result in context
BruterValue bruter_call(BruterList *context, BruterList *list)
{
    BruterValue(*_function)(BruterList*, BruterList*);
    if (context)
    {
        _function = context->data[list->data[0].i].p;
        return (BruterValue){.i = _function(context, list).i};
    }
    else 
    {
        _function = list->data[0].p;
        return _function(NULL, list);
    }
}
