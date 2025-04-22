// libbruter
// libbruter use no other libraries beside the standard C library
// if libbruter does not work on a platform, it is a bug, and should be reported.
// it is meant to be compatible even with arduino and wasm, both tested.
// so any platform that has a standard C library should be able to compile and run libbruter.
// even non-gcc-like compilers like cl.exe are expected to work.

#ifndef BRUTER_H
#define BRUTER_H 1

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <limits.h>

// standard library
#include <string.h>
#include <math.h>
#include <time.h>
#include <float.h>
#include <ctype.h>
#include <inttypes.h>

// version
#define VERSION "0.8.2"

typedef intptr_t Int;
typedef uintptr_t UInt;

// we use Int and Float instead of int and float because we need to use always the pointer size for any type that might share the fundamental union type;
// bruter use a union as universal type, and bruter is able to manipulate and use pointers direcly so we need to use the pointer size;
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
    
    // these types are arrays
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
    Int size;
    Int capacity;
} List;

List *list_init(Int size);
void list_free(List *list);
void list_double(List *list);
void list_half(List *list);
void list_push(List *list, Value value);
void list_unshift(List *list, Value value);
Value list_pop(List *list);
Value list_shift(List *list);
void list_swap(List *list, Int i1, Int i2);
void list_insert(List *list, Int i, Value value);
Value list_remove(List *list, Int i);
Value list_fast_remove(List *list, Int i);
Int list_ocurrences(List *list, Value value);
Int list_find(List *list, Value value);
void list_reverse(List *list);

typedef struct
{
    List *hashes;
    List *values;
} VirtualMachine;

//Function
typedef Int (*Function)(VirtualMachine*, List*);
typedef void (*InitFunction)(VirtualMachine*);

//String
char* str_duplicate(const char *str);
char* str_nduplicate(const char *str, UInt n);

char* str_format(const char *fmt, ...);

List* special_space_split(char *str);
List* special_split(char *str, char delim);

// variable
VirtualMachine* make_vm(Int size);
void free_vm(VirtualMachine *vm);

Int new_var(VirtualMachine *vm, char* varname);
Int new_block(VirtualMachine *vm, char* varname, Int size);

Int hash_find(VirtualMachine *vm, char *varname);
void hash_set(VirtualMachine *vm, char* varname, Int index);
void hash_unset(VirtualMachine *vm, char* varname);

// macros
#define data(index) (vm->values->data[index])
#define data_s(index) (&vm->values->data[index].u8[0])

#define hash(index) (vm->hashes->data[index].s)

#define arg(index) (vm->values->data[args->data[index].i])
#define arg_i(index) (args->data[index].i)
#define arg_s(index) (&vm->values->data[args->data[index].i].u8[0])

#define function(name) Int name(VirtualMachine *vm, List *args)
#define init(name) void init_##name(VirtualMachine *vm)

// eval
Int eval(VirtualMachine *vm, char *cmd);
Int interpret(VirtualMachine *vm, char *cmd);

// functions
List* parse(void* _vm, char* cmd);

#endif