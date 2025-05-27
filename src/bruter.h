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
#define VERSION "0.8.6"

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
#define BRUTER_VALUE(type, value) (BruterValue){.type = value}

typedef struct 
{
    BruterValue *data;
    char **keys;
    BruterInt size;
    BruterInt capacity;
} BruterList;

// BruterList functions(also work for tables as tables are just lists with keys)
BruterList   *bruter_init(BruterInt size, bool is_table);
void          bruter_free(BruterList *list);
void          bruter_double(BruterList *list);
void          bruter_half(BruterList *list);
void          bruter_push(BruterList *list, BruterValue value, const char* key);
void          bruter_unshift(BruterList *list, BruterValue value, const char* key);
void          bruter_insert(BruterList *list, BruterInt i, BruterValue value, const char* key);
BruterValue   bruter_pop(BruterList *list);
BruterValue   bruter_shift(BruterList *list);
BruterValue   bruter_remove(BruterList *list, BruterInt i);
BruterValue   bruter_fast_remove(BruterList *list, BruterInt i);
void          bruter_swap(BruterList *list, BruterInt i1, BruterInt i2);
BruterInt     bruter_find(BruterList *list, BruterValue value, const char* key);
void          bruter_reverse(BruterList *list);
// if context is NULL, it will call direcly from list->data[0].p and the result itself
// if context is not NULL, it will call from context->data[list->data[0].i].p and return the index of the result in context
// if context is not NULL, the result will be always an int, because it return the index of the result in context
BruterValue   bruter_call(BruterList *context, BruterList *list);

// experimental
BruterList   *bruter_copy(BruterList *list);

#define BRUTER_FUNCTION(name) BruterInt name(BruterList *context, BruterList *args)

#endif