// libbruter
// libbruter use no other libraries beside the standard C99 libraries
// if libbruter does not work on a platform, it is a bug, and should be reported.

#ifndef BRUTER_H
#define BRUTER_H 1

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>

// version
#define VERSION "0.8.8"

typedef intptr_t BruterInt;
typedef uintptr_t BruterUInt;

// BRUTER use BruterInt and BruterFloat instead of int and float because we need to use always the pointer size for any type that might share the fundamental union type;
// BRUTER use a union as universal type, and BRUTER is able to manipulate and use pointers direcly so we need to use the pointer size;
#if INTPTR_MAX == INT64_MAX
typedef double BruterFloat;
#else
typedef float BruterFloat;
#endif

// BruterValue
typedef union 
{
    // these types depend on the size of the pointer
    BruterFloat f;
    BruterInt i;
    BruterUInt u;

    // these types are pointers
    char* s;
    void* p;
    
    // these types are int arrays
    uint8_t u8[sizeof(BruterFloat)];
    uint16_t u16[sizeof(BruterFloat) / 2];
    uint32_t u32[sizeof(BruterFloat) / 4];

    int8_t i8[sizeof(BruterFloat)];
    int16_t i16[sizeof(BruterFloat) / 2];
    int32_t i32[sizeof(BruterFloat) / 4];

    float f32[sizeof(BruterFloat) / 4];
} BruterValue;

typedef struct 
{
    BruterValue **data;
    BruterInt data_count; // how many buffers are allocated
    BruterInt size;
    BruterInt capacity;
} BruterList;

#ifdef USE_SHORT_TYPES

#warning "USE_SHORT_TYPES is defined; using short type aliases like Int, Float, etc."

#define Int BruterInt
#define UInt BruterUInt
#define Float BruterFloat
#define Value BruterValue
#define List BruterList

#endif

static inline BruterValue bruter_value_i(BruterInt value);
static inline BruterValue bruter_value_u(BruterUInt value);
static inline BruterValue bruter_value_f(BruterFloat value);
static inline BruterValue bruter_value_p(void *value);

// BruterList functions(also work for tables as tables are just lists with keys)
static inline BruterList   *bruter_init(BruterInt size, BruterInt how_many_buffers);
static inline void          bruter_free(BruterList *list);
static inline void          bruter_double(BruterList *list);
static inline void          bruter_half(BruterList *list);
static inline void          bruter_push(BruterList *list, BruterValue value, ...);
static inline void          bruter_unshift(BruterList *list, BruterValue value, ...);
static inline void          bruter_insert(BruterList *list, BruterInt i, BruterValue value, ...);
static inline void          bruter_pop(BruterList *list);
static inline void          bruter_shift(BruterList *list);
static inline void          bruter_remove(BruterList *list, BruterInt i);
static inline void          bruter_fast_remove(BruterList *list, BruterInt i);
static inline void          bruter_swap(BruterList *list, BruterInt i1, BruterInt i2);
static inline BruterInt     bruter_find(BruterList *list, BruterInt buffer_index, BruterValue value);
static inline void          bruter_reverse(BruterList *list);
static inline BruterList   *bruter_copy(BruterList *list);
static inline void          bruter_concat(BruterList *dest, BruterList *src);
static inline BruterValue   bruter_call(BruterList *list);
static inline BruterValue   bruter_get(BruterList *list, BruterInt buffer_index, BruterInt i);
static inline void          bruter_set(BruterList *list, BruterInt buffer_index, BruterInt i, BruterValue value);

// functions implementations
// functions implementations
// functions implementations
// functions implementations

static inline BruterValue bruter_value_i(BruterInt value)
{
    return (BruterValue){.i = value};
}

static inline BruterValue bruter_value_u(BruterUInt value)
{
    return (BruterValue){.u = value};
}

static inline BruterValue bruter_value_f(BruterFloat value)
{
    return (BruterValue){.f = value};
}

static inline BruterValue bruter_value_p(void *value)
{
    return (BruterValue){.p = value};
}

static inline BruterList *bruter_init(BruterInt size, BruterInt how_many_buffers)
{
    BruterList *list = (BruterList*)malloc(sizeof(BruterList));
    if (!list)
    {
        printf("BRUTER_ERROR: failed to allocate memory for BruterList\n");
        exit(EXIT_FAILURE);
    }

    list->data = (BruterValue**)malloc(size * sizeof(BruterValue*));
    if (!list->data)
    {
        printf("BRUTER_ERROR: failed to allocate memory for BruterList data datalist\n");
        free(list);
        exit(EXIT_FAILURE);
    }

    for (BruterInt i = 0; i < how_many_buffers; i++)
    {
        list->data[i] = (BruterValue*)malloc(size * sizeof(BruterValue));
        if (!list->data[i])
        {
            printf("BRUTER_ERROR: failed to allocate memory for BruterList data buffer %d\n", i);
            for (BruterInt j = 0; j < i; j++)
            {
                free(list->data[j]);
            }
            free(list->data);
            free(list);
            exit(EXIT_FAILURE);
        }
    }
    list->data_count = how_many_buffers;
    list->size = 0;
    list->capacity = size;

    return list;
}

static inline void bruter_free(BruterList *list)
{
    for (BruterInt i = 0; i < list->data_count; i++)
    {
        free(list->data[i]);
    }
    free(list->data);
    free(list);
}

static inline void bruter_double(BruterList *list)
{
    list->capacity = list->capacity == 0 ? 1 : list->capacity * 2;
    for (BruterInt i = 0; i < list->data_count; i++)
    {
        BruterValue *new_data = realloc(list->data[i], list->capacity * sizeof(BruterValue));
        if (!new_data)
        {
            printf("BRUTER_ERROR: failed to reallocate list data buffer %d\n", i);
            exit(EXIT_FAILURE);
        }
        list->data[i] = new_data;
    }
}

static inline void bruter_half(BruterList *list)
{
    list->capacity /= 2;

    for (BruterInt i = 0; i < list->data_count; i++)
    {
        BruterValue *new_data = realloc(list->data[i], list->capacity * sizeof(BruterValue));
        if (!new_data)
        {
            printf("BRUTER_ERROR: failed to reallocate list data buffer %d\n", i);
            exit(EXIT_FAILURE);
        }
        list->data[i] = new_data;
    }
    if (list->size > list->capacity)
    {
        list->size = list->capacity; // ensure size does not exceed capacity
    }
    /*if (list->size == 0)
    {
        for (BruterInt i = 0; i < list->data_count; i++)
        {
            free(list->data[i]);
            list->data[i] = NULL;
        }
    }*/
}

static inline void bruter_push(BruterList *list, BruterValue value, ...)
{
    if (list->size == list->capacity)
    {
        bruter_double(list);
    }

    va_list args;
    va_start(args, value);
    
    list->data[0][list->size] = value; // push to the first buffer

    if (list->data_count > 1)
    {
        for (BruterInt i = 1; i < list->data_count; i++)
        {
            list->data[i][list->size] = va_arg(args, BruterValue); // push to the other buffers
        }
    }
    va_end(args);
    list->size++;
}

static inline void bruter_unshift(BruterList *list, BruterValue value, ...)
{
    if (list->size == list->capacity)
    {
        bruter_double(list);
    }
    
    va_list args;
    va_start(args, value);

    memmove(&(list->data[0][1]), &(list->data[0][0]), list->size * sizeof(BruterValue)); // shift data to the right
    list->data[0][0] = value; // insert at the beginning
    if (list->data_count > 1)
    {
        for (BruterInt i = 1; i < list->data_count; i++)
        {
            memmove(&(list->data[i][1]), &(list->data[i][0]), list->size * sizeof(BruterValue)); // shift data to the right in other buffers
            list->data[i][0] = va_arg(args, BruterValue); // insert at the beginning in other buffers
        }
    }
    va_end(args);
    list->size++;
}

static inline void bruter_insert(BruterList *list, BruterInt i, BruterValue value, ...)
{
    if (list->size == list->capacity)
    {
        bruter_double(list);
    }

    va_list args;
    va_start(args, value);

    if (i <= list->size) 
    {
        if (i < 0 || i >= list->size)
        {
            printf("BRUTER_ERROR: index %" PRIdPTR " out of range in list of size %" PRIdPTR " \n", i, list->size);
            exit(EXIT_FAILURE);
        }

        memmove(&(list->data[0][i + 1]), &(list->data[0][i]), (list->size - i) * sizeof(BruterValue)); // shift data to the right
        list->data[0][i] = value; // insert at index i

        if (list->data_count > 1)
        {
            for (BruterInt j = 1; j < list->data_count; j++)
            {
                memmove(&(list->data[j][i + 1]), &(list->data[j][i]), (list->size - i) * sizeof(BruterValue)); // shift data to the right in other buffers
                list->data[j][i] = va_arg(args, BruterValue); // insert at index i in other buffers
            }
        }
    } 
    else 
    {
        printf("BRUTER_ERROR: index %" PRIdPTR " out of range in list of size %" PRIdPTR " \n", i, list->size);
        exit(EXIT_FAILURE);
    }

    va_end(args);
    list->size++;
}

static inline void bruter_pop(BruterList *list)
{
    if (list->size == 0)
    {
        printf("BRUTER_ERROR: cannot pop from empty list\n");
        exit(EXIT_FAILURE);
    }
    
    list->size--;
}

static inline void bruter_shift(BruterList *list)
{
    memmove(&(list->data[0][0]), &(list->data[0][1]), (list->size - 1) * sizeof(BruterValue)); // shift data to the left
    if (list->data_count > 1)
    {
        for (BruterInt i = 1; i < list->data_count; i++)
        {
            memmove(&(list->data[i][0]), &(list->data[i][1]), (list->size - 1) * sizeof(BruterValue)); // shift data to the left
        }
    }
    list->size--;
}

static inline void bruter_remove(BruterList *list, BruterInt i)
{
    memmove(&(list->data[0][i]), &(list->data[0][i + 1]), (list->size - (i) - 1) * sizeof(BruterValue));
    if (list->data_count > 1)
    {
        for (BruterInt j = 1; j < list->data_count; j++)
        {
            memmove(&(list->data[j][i]), &(list->data[j][i + 1]), (list->size - (i) - 1) * sizeof(BruterValue));
        }
    }
    list->size--;
    
}

static inline void bruter_fast_remove(BruterList *list, BruterInt i)
{
    bruter_swap(list, i, list->size - 1);
    bruter_pop(list);
}

static inline void bruter_swap(BruterList *list, BruterInt i1, BruterInt i2)
{
    BruterValue tmp = list->data[0][i1];
    list->data[0][i1] = list->data[0][i2];
    list->data[0][i2] = tmp;
    if (list->data_count > 1)
    {
        for (BruterInt j = 1; j < list->data_count; j++)
        {
            tmp = list->data[j][i1];
            list->data[j][i1] = list->data[j][i2];
            list->data[j][i2] = tmp;
        }
    }
}

static inline BruterInt bruter_find(BruterList *list, BruterInt buffer_index, BruterValue value)
{
    if (buffer_index < 0 || buffer_index >= list->data_count)
    {
        printf("BRUTER_ERROR: buffer index %" PRIdPTR " out of range in list with %d buffers\n", buffer_index, list->data_count);
        exit(EXIT_FAILURE);
    }

    for (BruterInt i = 0; i < list->size; i++)
    {
        if (list->data[buffer_index][i].i == value.i)
        {
            return i;
        }
    }

    return -1;
}

static inline void bruter_reverse(BruterList *list)
{
    for (BruterInt i = 0; i < list->data_count; i++)
    {
        for (BruterInt j = 0; j < list->size / 2; j++)
        {
            BruterValue tmp = list->data[i][j];
            list->data[i][j] = list->data[i][list->size - j - 1];
            list->data[i][list->size - j - 1] = tmp;
        }
    }
}

static inline BruterList* bruter_copy(BruterList *list)
{
    BruterList *copy = bruter_init(list->capacity, list->data_count);
    copy->size = list->size;
    for (BruterInt i = 0; i < list->data_count; i++)
    {
        memcpy(copy->data[i], list->data[i], list->size * sizeof(BruterValue));
    }
    return copy;
}

static inline void bruter_concat(BruterList *dest, BruterList *src)
{
    if (dest->size + src->size > dest->capacity)
    {
        while (dest->size + src->size > dest->capacity)
        {
            bruter_double(dest);
        }
    }

    for (BruterInt i = 0; i < src->size; i++)
    {
        dest->data[0][dest->size + i] = src->data[0][i]; // copy to the first buffer
        if (dest->data_count > 1)
        {
            for (BruterInt j = 1; j < dest->data_count; j++)
            {
                dest->data[j][dest->size + i] = src->data[j][i]; // copy to the other buffers
            }
        }
    }
    dest->size += src->size;
}

static inline BruterValue bruter_call(BruterList *list)
{
    for (BruterInt i = 0; i < list->data_count; i++)
    {
        if (list->data[i][0].p != NULL) // assuming the first element is a function pointer
        {
            // call the function with the rest of the data as arguments
            BruterValue result = ((BruterValue(*)(BruterList*))list->data[i][0].p)(list);
            return result; // return the result of the function call
        }
        else 
        {
            printf("BRUTER_ERROR: first element of buffer %d is not a function pointer, it is NULL\n", i);
            exit(EXIT_FAILURE);
        }
    }
}


static inline BruterValue bruter_get(BruterList *list, BruterInt buffer_index, BruterInt i)
{
    if (i < 0 || i >= list->size)
    {
        return bruter_value_i(-1); // return -1 if index is out of range
    }
    return list->data[buffer_index][i];
}

static inline void bruter_set(BruterList *list, BruterInt buffer_index, BruterInt i, BruterValue value)
{
    if (i < 0 || i >= list->size)
    {
        printf("BRUTER_ERROR: index %" PRIdPTR " out of range in list of size %" PRIdPTR " \n", i, list->size);
        exit(EXIT_FAILURE);
    }
    list->data[buffer_index][i] = value;
}

#endif