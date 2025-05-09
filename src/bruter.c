#include "bruter.h"

List *list_init(Int size)
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
    list->keys = NULL;
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

void list_push(List *list, Value value)
{
    if (list->size == list->capacity)
    {
        list_double(list);
    }

    list->data[list->size] = value;
    list->size++;
}

void list_unshift(List *list, Value value)
{
    if (list->size == list->capacity)
    {
        list_double(list);
    }

    memmove(&(list->data[1]), &(list->data[0]), list->size * sizeof(Value));
    list->data[0] = value;
    list->size++;
}

void list_insert(List *list, Int i, Value value)
{
    if (list->size == list->capacity)
    {
        list_double(list);
    }

    if (i <= list->size) 
    {
        memmove(&(list->data[i + 1]), &(list->data[i]), (list->size - i) * sizeof(Value));
        list->data[i] = value;
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

Int list_find(List *list, Value value)
{
    for (Int i = 0; i < list->size; i++)
    {
        if (list->data[i].i == value.i)
        {
            return i;
        }
    }
    return -1;
}

void list_reverse(List *list)
{
    for (Int i = 0; i < list->size / 2; i++)
    {
        list_swap(list, i, list->size - i - 1);
    }
}

// pass NULL for context if you want to call a function directly
// if context exist, the return will be always an int, because it return the index of the result in context
Value list_call(List *context, List *list)
{
    Value(*_function)(List*, List*);
    if (context)
    {
        _function = context->data[list->data[0].i].p;
        return (Value){.i = _function(context, list).i};
    }
    else 
    {
        _function = list->data[0].p;
        return _function(NULL, list);
    }
}

// only for tables
List *table_init(Int size)
{
    List *list = list_init(size);
    list->keys = (char**)malloc(size * sizeof(char*));
    for (Int i = 0; i < size; i++)
    {
        list->keys[i] = NULL;
    }
    if (list->keys == NULL)
    {
        printf("BRUTER_ERROR: failed to allocate memory for List keys\n");
        free(list->data);
        free(list);
        exit(EXIT_FAILURE);
    }
    return list;
}

// only for tables 
void table_set(List *table, char* key, Value value)
{
    if (table->keys == 0)
    {
        printf("BRUTER_ERROR: list_table_set called on a non-table\n");
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
    table_push(table, value, key);
}

// only for tables 
Int table_find(List *table, char* key)
{
    if (table->keys == 0)
    {
        printf("BRUTER_ERROR: table_find called on a non-table\n");
        return -1;
    }
    else if (!key)
    {
        printf("BRUTER_ERROR: table_find called with NULL key\n");
        return -1;
    }

    for (Int i = 0; i < table->size; i++)
    {
        if (table->keys[i] && strcmp(table->keys[i], key) == 0)
        {
            return i;
        }
    }
    
    return -1;
}

void table_push(List *list, Value value, char* key)
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

void table_unshift(List *list, Value value, char* key)
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

void table_insert(List *list, Int i, Value value, char* key)
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
