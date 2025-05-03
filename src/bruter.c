#include "bruter.h"

List *list_init(Int size, bool istable)
{
    List *list = (List*)malloc(sizeof(List));
    
    if (list == NULL)
    {
        printf("BRUTER_ERROR: failed to allocate memory for List\n");
        exit(EXIT_FAILURE);
    }
    
    list->data = (size == 0) ? NULL : (Value*)malloc(size * sizeof(Value));
    
    if (size > 0 && list->data == NULL)
    {
        printf("BRUTER_ERROR: failed to allocate memory for List data\n");
        free(list);
        exit(EXIT_FAILURE);
    }
    
    list->size = 0;
    list->capacity = size;
    list->keys = istable ? (char**)malloc(size * sizeof(char*)) : NULL;
    if (istable)
    {
        for (Int i = 0; i < size; i++)
        {
            list->keys[i] = NULL;
        }
    }
    return list;
}

void list_free(List *list)
{
    free(list->data);
    if (list->keys != NULL)
    {
        for (Int i = 0; i < list->size; i++)
        {
            free(list->keys[i]);
        }
        free(list->keys);
    }
    free(list);
}

void list_double(List *list)
{
    list->capacity = list->capacity == 0 ? 1 : list->capacity * 2;
    list->data = realloc(list->data, list->capacity * sizeof(Value));
    if (list->keys != NULL)
    {
        list->keys = realloc(list->keys, list->capacity * sizeof(char*));
    }
}

void list_half(List *list)
{
    list->capacity /= 2;
    list->data = realloc(list->data, list->capacity * sizeof(Value));
    if (list->keys != NULL)
    {
        list->keys = realloc(list->keys, list->capacity * sizeof(char*));
    }

    if (list->size > list->capacity)
    {
        list->size = list->capacity;
    }
}

void list_push(List *list, Value value, char* key)
{
    if (list->size == list->capacity)
    {
        list_double(list);
    }
    list->data[list->size] = value;

    if (list->keys != NULL && key != NULL)
    {
        int len = strlen(key);
        list->keys[list->size] = malloc(len + 1);
        strncpy(list->keys[list->size], key, len);
        list->keys[list->size][len] = '\0';
    }
    list->size++;
}

void list_unshift(List *list, Value value, char* key)
{
    if (list->size == list->capacity)
    {
        list_double(list);
    }
    memmove(&(list->data[1]), &(list->data[0]), list->size * sizeof(Value));
    list->data[0] = value;
    if (list->keys != NULL && key != NULL)
    {
        memmove(&(list->keys[1]), &(list->keys[0]), list->size * sizeof(char*));
        int len = strlen(key);
        list->keys[0] = malloc(len + 1);
        strncpy(list->keys[0], key, len);
        list->keys[0][len] = '\0';
    }
    list->size++;
}

void list_insert(List *list, Int i, Value value, char* key)
{
    if (list->size == list->capacity)
    {
        list_double(list);
    }
    if (i <= list->size) 
    {
        memmove(&(list->data[i + 1]), &(list->data[i]), (list->size - i) * sizeof(Value));
        list->data[i] = value;
        if (list->keys != NULL && key != NULL)
        {
            memmove(&(list->keys[i + 1]), &(list->keys[i]), (list->size - i) * sizeof(char*));
            int len = strlen(key);
            list->keys[0] = malloc(len + 1);
            strncpy(list->keys[0], key, len);
            list->keys[0][len] = '\0';
        }
        list->size++;
    } 
    else 
    {
        printf("BRUTER_ERROR: index %" PRIdPTR " out of range in list of size %" PRIdPTR " \n", i, list->size);
        exit(EXIT_FAILURE);
    }
}

Value list_pop(List *list)
{
    if (list->keys != NULL)
    {
        free(list->keys[list->size - 1]);
        list->keys[list->size - 1] = NULL;
    }
    return list->data[--list->size];
}

Value list_shift(List *list)
{
    Value ret = list->data[0];
    if (list->size > 1) 
    {
        memmove(&(list->data[0]), &(list->data[1]), (list->size - 1) * sizeof(Value));
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

Value list_remove(List *list, Int i)
{
    Value ret = list->data[i];
    memmove(&(list->data[i]), &(list->data[i + 1]), (list->size - (i) - 1) * sizeof(Value));
    list->size--;
    if (list->keys != NULL)
    {
        free(list->keys[i]);
        list->keys[i] = NULL;
        memmove(&(list->keys[i]), &(list->keys[i + 1]), (list->size - (i) - 1) * sizeof(char*));
    }
    return ret;
}

Value list_fast_remove(List *list, Int i)
{
    Value ret = list->data[i];
    list_swap(list, i, list->size - 1);
    list_pop(list);
    return ret;
}

void list_swap(List *list, Int i1, Int i2)
{
    Value tmp = list->data[i1];
    list->data[i1] = list->data[i2];
    list->data[i2] = tmp;

    if (list->keys != NULL)
    {
        char* tmp_key = list->keys[i1];
        list->keys[i1] = list->keys[i2];
        list->keys[i2] = tmp_key;
    }
}

Int list_find(List *list, Value value, char* key)
{
    Int i = -1;
    if (key && list->keys)
    {
        for (Int j = 0; j < list->size; j++)
        {
            if (list->keys[j] && strcmp(list->keys[j], key) == 0)
            {
                i = j;
                break;
            }
        }
        return i;
    }
    else 
    {
        if (key)
        {
            printf("BRUTER_WARNING: you provided a key in list_find but the list is not a table\n");
        } 

        for (Int j = 0; j < list->size; j++)
        {
            if (list->data[j].i == value.i)
            {
                i = j;
                break;
            }
        }
    }
    return i;
}

void list_reverse(List *list)
{
    for (Int i = 0; i < list->size / 2; i++)
    {
        list_swap(list, i, list->size - i - 1);
    }
}

Value list_call(List *list)
{
    Value(*_function)(List*) = list->data[0].p;
    return _function(list);
}

// only for tables 
void list_set(List *table, char* key, Value value)
{
    if (table->keys == 0)
    {
        printf("BRUTER_ERROR: list_set called on a non-table\n");
        return;
    }

    for (Int i = 0; i < table->size; i++)
    {
        if (table->keys[i] && strcmp(table->keys[i], key) == 0)
        {
            table->data[i] = value;
            return;
        }
    }
    
    // if not found, add it
    list_push(table, value, key);
}