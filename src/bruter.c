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
    free(list);
}

void list_double(List *list)
{
    list->capacity = list->capacity == 0 ? 1 : list->capacity * 2;
    list->data = realloc(list->data, list->capacity * sizeof(Value));
}

void list_half(List *list)
{
    list->capacity /= 2;
    list->data = realloc(list->data, list->capacity * sizeof(Value));
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
    list->data[list->size++] = value;
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

Value list_pop(List *list)
{
    return list->data[--list->size];
}

Value list_shift(List *list)
{
    Value ret = list->data[0];
    if (list->size > 1) 
    {
        memmove(&(list->data[0]), &(list->data[1]), (list->size - 1) * sizeof(Value));
    }
    list->size--;
    return ret;
}


void list_swap(List *list, Int i1, Int i2)
{
    Value tmp = list->data[i1];
    list->data[i1] = list->data[i2];
    list->data[i2] = tmp;
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
        printf("BRUTER_ERROR: index %" PRIdPTR "  out of range in list of size %" PRIdPTR " \n", i, list->size);
        exit(EXIT_FAILURE);
    }
}

Value list_remove(List *list, Int i)
{
    Value ret = list->data[i];
    memmove(&(list->data[i]), &(list->data[i + 1]), (list->size - (i) - 1) * sizeof(Value));
    list->size--;
    return ret;
}

Value list_fast_remove(List *list, Int i)
{
    Value ret = list->data[i];
    list_swap(list, i, list->size - 1);
    list_pop(list);
    return ret;
}

Int list_ocurrences(List *list, Value value)
{
    Int i = 0;
    while (i < list->size && list->data[i].i != value.i)
    {
        i++;
    }
    return i == list->size ? -1 : i;
}

Int list_find(List *list, Value value)
{
    Int i = -1;
    for (Int j = 0; j < list->size; j++)
    {
        if (list->data[j].i == value.i)
        {
            i = j;
            break;
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

Int list_call(List *context, List* args)
{
    if (!args->size)
    {
        list_free(args);
        return -1;
    }
    
    Int func = list_shift(args).i;
    Int result = -1;
    if (func > -1)
    {
        Function _function = context->data[func].p;
        result = _function(context, args);
    }
    else
    {
        printf("BRUTER_ERROR: %" PRIdPTR " is not a function\n", func);
    }
    
    return result;
}

// tables functions

List *table_init(Int size)
{
    List *table = (List*)malloc(sizeof(List));
    
    if (table == NULL)
    {
        printf("BRUTER_ERROR: failed to allocate memory for List\n");
        exit(EXIT_FAILURE);
    }
    
    table->data = (size == 0) ? NULL : (Value*)malloc(size * sizeof(Value));
    table->keys = (size == 0) ? NULL : (char**)malloc(size * sizeof(char*));
    
    if (size > 0 && table->data == NULL)
    {
        printf("BRUTER_ERROR: failed to allocate memory for List data\n");
        free(table);
        exit(EXIT_FAILURE);
    }
    
    table->keys = (char**)malloc(size * sizeof(char*));
    
    if (size > 0 && table->keys == NULL)
    {
        printf("BRUTER_ERROR: failed to allocate memory for List keys\n");
        free(table->data);
        free(table);
        exit(EXIT_FAILURE);
    }
    
    table->size = 0;
    table->capacity = size;
    return table;
}

void table_free(List *table)
{
    free(table->data);
    for (Int i = 0; i < table->size; i++)
    {
        if (table->keys[i] != NULL)
        {
            free(table->keys[i]);
        }
    }
    free(table->keys);
    free(table);
}

void table_double(List *table)
{
    table->capacity = table->capacity == 0 ? 1 : table->capacity * 2;
    table->data = realloc(table->data, table->capacity * sizeof(Value));
    table->keys = realloc(table->keys, table->capacity * sizeof(char*));
}

void table_half(List *table)
{
    table->capacity /= 2;
    table->data = realloc(table->data, table->capacity * sizeof(Value));
    table->keys = realloc(table->keys, table->capacity * sizeof(char*));
    if (table->size > table->capacity)
    {
        table->size = table->capacity;
    }
}

void table_push(List *table, Value value, char* key)
{
    if (table->size == table->capacity)
    {
        table_double(table);
    }
    table->data[table->size] = value;
    table->keys[table->size] = key;
    table->size++;
}

void table_unshift(List *table, Value value, char* key)
{
    if (table->size == table->capacity)
    {
        table_double(table);
    }
    memmove(&(table->data[1]), &(table->data[0]), table->size * sizeof(Value));
    memmove(&(table->keys[1]), &(table->keys[0]), table->size * sizeof(char*));
    table->data[0] = value;
    table->keys[0] = key;
    table->size++;
}

void table_insert(List *table, Int i, Value value, char* key)
{
    if (table->size == table->capacity)
    {
        table_double(table);
    }
    if (i <= table->size) 
    {
        memmove(&(table->data[i + 1]), &(table->data[i]), (table->size - i) * sizeof(Value));
        memmove(&(table->keys[i + 1]), &(table->keys[i]), (table->size - i) * sizeof(char*));
        table->data[i] = value;
        table->keys[i] = key;
        table->size++;
    } 
    else 
    {
        printf("BRUTER_ERROR: index %" PRIdPTR " out of range in table of size %" PRIdPTR " \n", i, table->size);
        exit(EXIT_FAILURE);
    }
}

Value table_pop(List *table)
{
    free(table->keys[table->size - 1]);
    table->keys[table->size - 1] = NULL;
    return table->data[--table->size];
}

Value table_shift(List *table)
{
    Value ret = table->data[0];
    free(table->keys[0]);
    table->keys[0] = NULL;
    if (table->size > 1) 
    {
        memmove(&(table->data[0]), &(table->data[1]), (table->size - 1) * sizeof(Value));
        memmove(&(table->keys[0]), &(table->keys[1]), (table->size - 1) * sizeof(char*));
    }
    table->size--;
    return ret;
}

void table_swap(List *table, Int i1, Int i2)
{
    Value tmp = table->data[i1];
    table->data[i1] = table->data[i2];
    table->data[i2] = tmp;
    
    char* tmp_key = table->keys[i1];
    table->keys[i1] = table->keys[i2];
    table->keys[i2] = tmp_key;
}

Value table_remove(List *table, Int i)
{
    Value ret = table->data[i];
    free(table->keys[i]);
    table->keys[i] = NULL;
    memmove(&(table->data[i]), &(table->data[i + 1]), (table->size - (i) - 1) * sizeof(Value));
    memmove(&(table->keys[i]), &(table->keys[i + 1]), (table->size - (i) - 1) * sizeof(char*));
    table->size--;
    return ret;
}

Value table_fast_remove(List *table, Int i)
{
    Value ret = table->data[i];
    free(table->keys[i]);
    table->keys[i] = NULL;
    table_swap(table, i, table->size - 1);
    table_pop(table);
    return ret;
}

Int table_ocurrences(List *table, Value value)
{
    Int i = 0;
    while (i < table->size && table->data[i].i != value.i)
    {
        i++;
    }
    return i == table->size ? -1 : i;
}

Int table_find(List *table, Value value)
{
    Int i = -1;
    for (Int j = 0; j < table->size; j++)
    {
        if (table->data[j].i == value.i)
        {
            i = j;
            break;
        }
    }
    return i;
}

void table_reverse(List *table)
{
    for (Int i = 0; i < table->size / 2; i++)
    {
        table_swap(table, i, table->size - i - 1);
    }
}

Value table_get(List *table, char* key)
{
    for (Int i = 0; i < table->size; i++)
    {
        if (strcmp(table->keys[i], key) == 0)
        {
            return table->data[i];
        }
    }
    return (Value){.i = -1};
}

void table_set(List *table, char* key, Value value)
{
    for (Int i = 0; i < table->size; i++)
    {
        if (strcmp(table->keys[i], key) == 0)
        {
            table->data[i] = value;
            return;
        }
    }
    
    // if not found, add it
    table_push(table, value, key);
}

Int table_call(List *context, List* args)
{
    if (!args->size)
    {
        table_free(args);
        return -1;
    }
    
    Int func = table_shift(args).i;
    Int result = -1;
    if (func > -1)
    {
        Function _function = context->data[func].p;
        result = _function(context, args);
    }
    else
    {
        printf("BRUTER_ERROR: %" PRIdPTR " is not a function\n", func);
    }
    
    return result;
}