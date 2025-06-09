// libbruter
// libbruter use no other libraries beside the standard C99 libraries
// if libbruter does not work on a platform, it is a bug, and should be reported.

#ifndef BRUTER_H
#define BRUTER_H 1

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
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
    BruterValue *data;
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

static inline BruterList   *bruter_init(BruterInt size);
static inline void          bruter_free(BruterList *list);
static inline void          bruter_double(BruterList *list);
static inline void          bruter_half(BruterList *list);
static inline void          bruter_push(BruterList *list, BruterValue value);
static inline void          bruter_unshift(BruterList *list, BruterValue value);
static inline void          bruter_insert(BruterList *list, BruterInt i, BruterValue value);
static inline BruterValue   bruter_pop(BruterList *list);
static inline BruterValue   bruter_shift(BruterList *list);
static inline BruterValue   bruter_remove(BruterList *list, BruterInt i);
static inline BruterValue   bruter_fast_remove(BruterList *list, BruterInt i);
static inline void          bruter_swap(BruterList *list, BruterInt i1, BruterInt i2);
static inline BruterInt     bruter_find(BruterList *list, BruterValue value);
static inline void          bruter_reverse(BruterList *list);
static inline BruterList   *bruter_copy(BruterList *list);
static inline void          bruter_concat(BruterList *dest, BruterList *src);
static inline BruterValue   bruter_call(BruterList *list);

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

static inline BruterList *bruter_init(BruterInt size)
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

    return list;
}

static inline void bruter_free(BruterList *list)
{
    free(list->data);
    free(list);
}

static inline void bruter_double(BruterList *list)
{
    list->capacity = list->capacity == 0 ? 1 : list->capacity * 2;
    BruterValue *new_data = realloc(list->data, list->capacity * sizeof(BruterValue));
    if (!new_data)
    {
        printf("BRUTER_ERROR: failed to reallocate list data\n");
        exit(EXIT_FAILURE);
    }
    list->data = new_data;
}

static inline void bruter_half(BruterList *list)
{
    list->capacity /= 2;

    BruterValue *new_data = realloc(list->data, list->capacity * sizeof(BruterValue));
    if (!new_data)
    {
        printf("BRUTER_ERROR: failed to reallocate list data\n");
        exit(EXIT_FAILURE);
    }
    list->data = new_data;

    if (list->size > list->capacity)
    {
        list->size = list->capacity;
    }
}

static inline void bruter_push(BruterList *list, BruterValue value)
{
    if (list->size == list->capacity)
    {
        bruter_double(list);
    }
    list->data[list->size] = value;

    list->size++;
}

static inline void bruter_unshift(BruterList *list, BruterValue value)
{
    if (list->size == list->capacity)
    {
        bruter_double(list);
    }
    memmove(&(list->data[1]), &(list->data[0]), list->size * sizeof(BruterValue));
    list->data[0] = value;
    list->size++;
}

static inline void bruter_insert(BruterList *list, BruterInt i, BruterValue value)
{
    if (list->size == list->capacity)
    {
        bruter_double(list);
    }

    if (i <= list->size) 
    {
        memmove(&(list->data[i + 1]), &(list->data[i]), (list->size - i) * sizeof(BruterValue));
        list->data[i] = value;
        list->size++;
    } 
    else 
    {
        printf("BRUTER_ERROR: index %" PRIdPTR " out of range in list of size %" PRIdPTR " \n", i, list->size);
        exit(EXIT_FAILURE);
    }
}

static inline BruterValue bruter_pop(BruterList *list)
{
    if (list->size == 0)
    {
        printf("BRUTER_ERROR: cannot pop from empty list\n");
        exit(EXIT_FAILURE);
    }
    
    return list->data[--list->size];
}

static inline BruterValue bruter_shift(BruterList *list)
{
    BruterValue ret = list->data[0];
    if (list->size > 1) 
    {
        memmove(&(list->data[0]), &(list->data[1]), (list->size - 1) * sizeof(BruterValue));
    }

    list->size--;
    return ret;
}

static inline BruterValue bruter_remove(BruterList *list, BruterInt i)
{
    BruterValue ret = list->data[i];
    memmove(&(list->data[i]), &(list->data[i + 1]), (list->size - (i) - 1) * sizeof(BruterValue));
    list->size--;
    return ret;
}

static inline BruterValue bruter_fast_remove(BruterList *list, BruterInt i)
{
    BruterValue ret = list->data[i];
    bruter_swap(list, i, list->size - 1);
    bruter_pop(list);
    return ret;
}

static inline void bruter_swap(BruterList *list, BruterInt i1, BruterInt i2)
{
    BruterValue tmp = list->data[i1];
    list->data[i1] = list->data[i2];
    list->data[i2] = tmp;
}

static inline BruterInt bruter_find(BruterList *list, BruterValue value)
{
    for (BruterInt i = 0; i < list->size; i++)
    {
        if (list->data[i].i == value.i)
        {
            return i;
        }
    }
    return -1;
}

static inline void bruter_reverse(BruterList *list)
{
    for (BruterInt i = 0; i < list->size / 2; i++)
    {
        bruter_swap(list, i, list->size - i - 1);
    }
}

static inline BruterList* bruter_copy(BruterList *list)
{
    BruterList *copy = bruter_init(list->capacity);
    if (copy == NULL)
    {
        printf("BRUTER_ERROR: failed to allocate memory for BruterList copy\n");
        exit(EXIT_FAILURE);
    }
    copy->size = list->size;
    memcpy(copy->data, list->data, copy->size * sizeof(BruterValue));
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
    
    memcpy(&(dest->data[dest->size]), src->data, src->size * sizeof(BruterValue));
    dest->size += src->size;
}

// pass NULL for context if you want to call a function directly
// if context exist, the return will be always an int, because it return the index of the result in context
static inline BruterValue bruter_call(BruterList *list)
{
    BruterValue(*_function)(BruterList*);
    _function = list->data[0].p;
    return _function(list);
}

#endif