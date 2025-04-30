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