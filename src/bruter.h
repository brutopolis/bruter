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

// version
#define VERSION "0.8.4"

typedef intptr_t Int;
typedef uintptr_t UInt;

// BRUTER use Int and Float instead of int and float because we need to use always the pointer size for any type that might share the fundamental union type;
// BRUTER use a union as universal type, and BRUTER is able to manipulate and use pointers direcly so we need to use the pointer size;
#if INTPTR_MAX == INT64_MAX
typedef double Float;
#else
typedef float Float;
#endif

typedef uint8_t Byte;

// Value
typedef union 
{
    // these types depend on the size of the pointer
    Float f;
    Int i;
    UInt u;

    // these types are pointers
    char* s;
    void* p;
    
    // these types are int arrays
    uint8_t u8[sizeof(Float)];
    uint16_t u16[sizeof(Float) / 2];
    uint32_t u32[sizeof(Float) / 4];

    int8_t i8[sizeof(Float)];
    int16_t i16[sizeof(Float) / 2];
    int32_t i32[sizeof(Float) / 4];

    float f32[sizeof(Float) / 4];
} Value;

typedef struct 
{
    Value *data;
    char **keys;
    Int size;
    Int capacity;
} List;

List   *list_init(Int size);
void    list_free(List *list);
void    list_double(List *list);
void    list_half(List *list);
void    list_push(List *list, Value value);
void    list_unshift(List *list, Value value);
Value   list_pop(List *list);
Value   list_shift(List *list);
void    list_swap(List *list, Int i1, Int i2);
void    list_insert(List *list, Int i, Value value);
Value   list_remove(List *list, Int i);
Value   list_fast_remove(List *list, Int i);
Int     list_ocurrences(List *list, Value value);
Int     list_find(List *list, Value value);
void    list_reverse(List *list);
Int     list_call(List *context, List* args);

List  *table_init(Int size);
void    table_free(List *list);
void    table_double(List *list);
void    table_half(List *list);
void    table_push(List *list, Value value, char* key);
void    table_unshift(List *list, Value value, char* key);
void    table_insert(List *list, Int i, Value value, char* key);
Value   table_remove(List *list, Int i);
Value   table_pop(List *list);
Value   table_shift(List *list);
void    table_swap(List *list, Int i1, Int i2);
Value   table_fast_remove(List *list, Int i);
Int     table_ocurrences(List *list, Value value);
Int     table_find(List *list, Value value);
void    table_reverse(List *list);
Value   table_get(List *table, char* key);
void    table_set(List *table, char* key, Value value);
Int     table_call(List *context, List* args);

//Function
typedef Int (*Function)(List*, List*);

#define function(name) Int name(List *context, List *args)

#endif