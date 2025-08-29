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
#include <ctype.h>

// version
#define BRUTER_VERSION "0.9.2"

typedef intptr_t BruterInt;
typedef uintptr_t BruterUInt;

// BRUTER use BruterInt and BruterFloat instead of int and float because we need to use always the pointer size for any type that might share the fundamental union type;
// BRUTER use a union as universal type, and BRUTER is able to manipulate and use pointers direcly so we need to use the pointer size;
#if INTPTR_MAX == INT64_MAX
    typedef double BruterFloat;
#else
    typedef float BruterFloat;
#endif

// you can build bruter as lib, just define BRUTER_AS_SOURCE before including this header, it will act like a source.c file
#if defined(BRUTER_AS_SOURCE) 
    #define STATIC_INLINE
#else
    #define STATIC_INLINE static inline
#endif

#ifndef BRUTER_DEFAULT_SIZE
    #define BRUTER_DEFAULT_SIZE 0
#endif


// well, we need to declare the types before, so we can use them in the union
// this is not defined if BRUTER_MANUAL_UNION is defined, so you can define your own types BEFORE including this header
// note you'll need everything below to be defined before including this header too
typedef union  BruterValue BruterValue;
typedef struct BruterMeta BruterMeta;
typedef struct BruterList BruterList;

// if you are under a pure C99 eviroment, beware of the use of the union,
// C99 defines that the used type is always the last it was defined to
// but 99% of the time you wont have to care about this, all compilers i tested deal with this very well, even under very restricted environments;
// BruterValue
union BruterValue
{
    // all types depend on the size of the pointer
    BruterInt i;
    BruterFloat f;
    BruterUInt u;
    void* p;
};

struct BruterMeta
{
    BruterValue value;
    char *key; // the key, if it is a table
    int8_t type; // the type of the value, if it is typed
};

struct BruterList
{
    // the real size
    BruterInt capacity;
    // the current size
    BruterInt size;
    // if it is a table, the keys will be used, if it is a list, the keys will be NULL
    char **keys;
    // if typed, the types will be used, if not, it will be NULL
    int8_t *types;
    // the data, it is a pointer to a BruterValue array
    BruterValue *data;
};

typedef void (*Function)(BruterList *stack);

#define function(name) \
    void name(BruterList *stack)

#define init(name) \
    void init_##name(BruterList *context)

enum {
    BRUTER_TYPE_NULL = 0,
    BRUTER_TYPE_ANY,
    BRUTER_TYPE_FLOAT,
    BRUTER_TYPE_BUFFER,
    BRUTER_TYPE_LIST,
    BRUTER_TYPE_FUNCTION,
};

STATIC_INLINE BruterValue        bruter_value_int(BruterInt value);
STATIC_INLINE BruterValue        bruter_value_uint(BruterUInt value);
STATIC_INLINE BruterValue        bruter_value_float(BruterFloat value);
STATIC_INLINE BruterValue        bruter_value_pointer(void *value);
   
// BruterList functions   
// create a new list with the given size, if size is 0, it will be initialized with NULL data and then allocated when needed
STATIC_INLINE BruterList*        bruter_new(BruterInt size, bool is_table, bool is_typed);
// free the list    
STATIC_INLINE void               bruter_free(BruterList *list);
// double the list capacity   
STATIC_INLINE void               bruter_double(BruterList *list);
// halve the list capacity   
STATIC_INLINE void               bruter_half(BruterList *list);
// push a value to the end of the list
STATIC_INLINE void               bruter_push(BruterList *list, BruterValue value, const char* key, int8_t type);
STATIC_INLINE void               bruter_push_int(BruterList *list, BruterInt value, const char* key, int8_t type);
STATIC_INLINE void               bruter_push_uint(BruterList *list, BruterUInt value, const char* key, int8_t type);
STATIC_INLINE void               bruter_push_float(BruterList *list, BruterFloat value, const char* key, int8_t type);
STATIC_INLINE void               bruter_push_pointer(BruterList *list, void *value, const char* key, int8_t type);
STATIC_INLINE void               bruter_push_meta(BruterList *list, BruterMeta value);
// unshift a value to the start of the list
STATIC_INLINE void               bruter_unshift(BruterList *list, BruterValue value, const char* key, int8_t type);
STATIC_INLINE void               bruter_unshift_int(BruterList *list, BruterInt value, const char* key, int8_t type);
STATIC_INLINE void               bruter_unshift_uint(BruterList *list, BruterUInt value, const char* key, int8_t type);
STATIC_INLINE void               bruter_unshift_float(BruterList *list, BruterFloat value, const char* key, int8_t type);
STATIC_INLINE void               bruter_unshift_pointer(BruterList *list, void *value, const char* key, int8_t type);
STATIC_INLINE void               bruter_unshift_meta(BruterList *list, BruterMeta value);
// insert a value at index i in the list
STATIC_INLINE void               bruter_insert(BruterList *list, BruterInt i, BruterValue value, const char* key, int8_t type);
STATIC_INLINE void               bruter_insert_int(BruterList *list, BruterInt i, BruterInt value, const char* key, int8_t type);
STATIC_INLINE void               bruter_insert_uint(BruterList *list, BruterInt i, BruterUInt value, const char* key, int8_t type);
STATIC_INLINE void               bruter_insert_float(BruterList *list, BruterInt i, BruterFloat value, const char* key, int8_t type);
STATIC_INLINE void               bruter_insert_pointer(BruterList *list, BruterInt i, void *value, const char* key, int8_t type);
STATIC_INLINE void               bruter_insert_meta(BruterList *list, BruterInt i, BruterMeta value);
// define, search for a key in the list, if it exists, update it, if not, add it
STATIC_INLINE void               bruter_define(BruterList *list, BruterValue value, const char* key, int8_t type);
STATIC_INLINE void               bruter_define_int(BruterList *list, BruterInt value, const char* key, int8_t type);
STATIC_INLINE void               bruter_define_uint(BruterList *list, BruterUInt value, const char* key, int8_t type);
STATIC_INLINE void               bruter_define_float(BruterList *list, BruterFloat value, const char* key, int8_t type);
STATIC_INLINE void               bruter_define_pointer(BruterList *list, void *value, const char* key, int8_t type);
STATIC_INLINE void               bruter_define_meta(BruterList *list, BruterMeta value);
// pop a value from the end of the list
STATIC_INLINE BruterValue        bruter_pop(BruterList *list);
STATIC_INLINE BruterInt          bruter_pop_int(BruterList *list);
STATIC_INLINE BruterUInt         bruter_pop_uint(BruterList *list);
STATIC_INLINE BruterFloat        bruter_pop_float(BruterList *list);
STATIC_INLINE void*              bruter_pop_pointer(BruterList *list);
STATIC_INLINE BruterMeta    bruter_pop_meta(BruterList *list);
// shift a value from the start of the list
STATIC_INLINE BruterValue        bruter_shift(BruterList *list);
STATIC_INLINE BruterInt          bruter_shift_int(BruterList *list);
STATIC_INLINE BruterUInt         bruter_shift_uint(BruterList *list);
STATIC_INLINE BruterFloat        bruter_shift_float(BruterList *list);
STATIC_INLINE void*              bruter_shift_pointer(BruterList *list);
STATIC_INLINE BruterMeta    bruter_shift_meta(BruterList *list);
// remove and return the value at index i in the list, shifting the rest of the list
STATIC_INLINE BruterValue        bruter_remove(BruterList *list, BruterInt i);
STATIC_INLINE BruterInt          bruter_remove_int(BruterList *list, BruterInt i);
STATIC_INLINE BruterUInt         bruter_remove_uint(BruterList *list, BruterInt i);
STATIC_INLINE BruterFloat        bruter_remove_float(BruterList *list, BruterInt i);
STATIC_INLINE void*              bruter_remove_pointer(BruterList *list, BruterInt i);
STATIC_INLINE BruterMeta    bruter_remove_meta(BruterList *list, BruterInt i);
// fast remove a value at index i in the list, swapping it with the last element and popping it
STATIC_INLINE BruterValue        bruter_fast_remove(BruterList *list, BruterInt i);
STATIC_INLINE BruterInt          bruter_fast_remove_int(BruterList *list, BruterInt i);
STATIC_INLINE BruterUInt         bruter_fast_remove_uint(BruterList *list, BruterInt i);
STATIC_INLINE BruterFloat        bruter_fast_remove_float(BruterList *list, BruterInt i);
STATIC_INLINE void*              bruter_fast_remove_pointer(BruterList *list, BruterInt i);
STATIC_INLINE BruterMeta    bruter_fast_remove_meta(BruterList *list, BruterInt i);
// undefine, search for a key in the list, if it exists, remove it
STATIC_INLINE BruterValue        bruter_undefine(BruterList *list, const char* key);
STATIC_INLINE BruterInt          bruter_undefine_int(BruterList *list, const char* key);
STATIC_INLINE BruterUInt         bruter_undefine_uint(BruterList *list, const char* key);
STATIC_INLINE BruterFloat        bruter_undefine_float(BruterList *list, const char* key);
STATIC_INLINE void*              bruter_undefine_pointer(BruterList *list, const char* key);
STATIC_INLINE BruterMeta    bruter_undefine_meta(BruterList *list, const char* key);
// swap two values in the list at indices i1 and i2
STATIC_INLINE void               bruter_swap(BruterList *list, BruterInt i1, BruterInt i2);
// find the index of a key in the list, returns -1 if not found
STATIC_INLINE BruterInt          bruter_find_key(const BruterList *list, const char* key);
// find the index of a value in the list, returns -1 if not found
STATIC_INLINE BruterInt          bruter_find(const BruterList *list, BruterValue value);
STATIC_INLINE BruterInt          bruter_find_int(const BruterList *list, BruterInt value);
STATIC_INLINE BruterInt          bruter_find_uint(const BruterList *list, BruterUInt value);
STATIC_INLINE BruterInt          bruter_find_float(const BruterList *list, BruterFloat value);
STATIC_INLINE BruterInt          bruter_find_pointer(const BruterList *list, void *value);
STATIC_INLINE BruterInt          bruter_find_meta(const BruterList *list, BruterMeta value);
// reverse the list in place   
STATIC_INLINE void               bruter_reverse(BruterList *list);
// create a copy of the list, with the same capacity and size, but new data array
STATIC_INLINE BruterList*        bruter_copy(const BruterList *list);
// get a value at index i in the list, returns a value with i set to -1 if index is out of range
STATIC_INLINE BruterValue        bruter_get(const BruterList *list, BruterInt i);
STATIC_INLINE BruterInt          bruter_get_int(const BruterList *list, BruterInt i);
STATIC_INLINE BruterUInt         bruter_get_uint(const BruterList *list, BruterInt i);
STATIC_INLINE BruterFloat        bruter_get_float(const BruterList *list, BruterInt i);
STATIC_INLINE void*              bruter_get_pointer(const BruterList *list, BruterInt i);
STATIC_INLINE BruterMeta    bruter_get_meta(const BruterList *list, BruterInt i);
// set a value at index i in the list, if index is out of range, it will print an error and exit
STATIC_INLINE void               bruter_set(BruterList *list, BruterInt i, BruterValue value, const char* key, int8_t type);
STATIC_INLINE void               bruter_set_int(BruterList *list, BruterInt i, BruterInt value, const char* key, int8_t type);
STATIC_INLINE void               bruter_set_uint(BruterList *list, BruterInt i, BruterUInt value, const char* key, int8_t type);
STATIC_INLINE void               bruter_set_float(BruterList *list, BruterInt i, BruterFloat value, const char* key, int8_t type);
STATIC_INLINE void               bruter_set_pointer(BruterList *list, BruterInt i, void *value, const char* key, int8_t type);
STATIC_INLINE void               bruter_set_meta(BruterList *list, BruterInt i, BruterMeta value);
// get a key at index i in the list, returns NULL if index is out of range or if the list is not a table
STATIC_INLINE char*              bruter_get_key(const BruterList *list, BruterInt i);
// set a key at index i in the list, if index is out of range, it will print an error and exit
STATIC_INLINE void               bruter_set_key(const BruterList *list, BruterInt i , const char *key);
// get a type at index i in the list, returns 0 if index is out of range or if the list is not typed
STATIC_INLINE int8_t             bruter_get_type(const BruterList *list, BruterInt i);
// set a type at index i in the list, if index is out of range, it will print an error and exit
STATIC_INLINE void               bruter_set_type(const BruterList *list, BruterInt i, int8_t type);
// get the bruter version   
STATIC_INLINE const char*        bruter_get_version(void);
// arena   
STATIC_INLINE void*              bruter_alloc(BruterList *arena, size_t size);
// bruter representation
STATIC_INLINE void               bruter_interpret(BruterList *context, const char* input_str, BruterList* splited, BruterList* stack);

// functions implementations
// functions implementations
// functions implementations
// functions implementations
#ifndef BRUTER_AS_HEADER // you can define this to not include the implementations

STATIC_INLINE BruterValue bruter_value_int(BruterInt value)
{
    return (BruterValue){.i = value};
}

STATIC_INLINE BruterValue bruter_value_uint(BruterUInt value)
{
    return (BruterValue){.u = value};
}

STATIC_INLINE BruterValue bruter_value_float(BruterFloat value)
{
    return (BruterValue){.f = value};
}

STATIC_INLINE BruterValue bruter_value_pointer(void *value)
{
    return (BruterValue){.p = value};
}

STATIC_INLINE BruterList *bruter_new(BruterInt size, bool is_table, bool is_typed)
{
    BruterList *list = (BruterList*)malloc(sizeof(BruterList));
    
    if (list == NULL)
    {
        printf("BRUTER_ERROR: failed to allocate memory for BruterList\n");
        exit(EXIT_FAILURE);
    }
    
    list->data = (size == 0) ? NULL : (BruterValue*)malloc((size_t)size * sizeof(BruterValue));
    
    if (size > 0 && list->data == NULL)
    {
        printf("BRUTER_ERROR: failed to allocate memory for BruterList data\n");
        exit(EXIT_FAILURE);
    }
    
    list->size = 0;
    list->capacity = size;

    if (is_table)
    {
        list->keys = (char**)calloc((size_t)size, sizeof(char*));
        if (list->keys == NULL)
        {
            printf("BRUTER_ERROR: failed to allocate memory for BruterList keys\n");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        list->keys = NULL;
    }

    if (is_typed)
    {
        list->types = (int8_t*)calloc((size_t)size, sizeof(int8_t));
        if (list->types == NULL)
        {
            printf("BRUTER_ERROR: failed to allocate memory for BruterList types\n");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        list->types = NULL;
    }

    return list;
}

STATIC_INLINE void bruter_free(BruterList *list)
{
    if (list->keys != NULL)
    {
        for (BruterInt i = 0; i < list->size; i++)
        {
            free(list->keys[i]);
        }

        free(list->keys);
    }
    free(list->types);
    free(list->data);
    free(list);
}

STATIC_INLINE void bruter_double(BruterList *list)
{
    BruterValue *new_data = NULL;
    list->capacity = list->capacity == 0 ? 1 : list->capacity * 2;
    new_data = (BruterValue*)realloc(list->data, (size_t)list->capacity * sizeof(BruterValue));
    if (!new_data)
    {
        printf("BRUTER_ERROR: failed to reallocate list data\n");
        exit(EXIT_FAILURE);
    }
    list->data = new_data;

    if (list->keys != NULL)
    {
        char **new_keys = (char**)realloc(list->keys, (size_t)list->capacity * sizeof(char*));
        if (!new_keys)
        {
            printf("BRUTER_ERROR: failed to reallocate list keys\n");
            exit(EXIT_FAILURE);
        }
        list->keys = new_keys;
        memset(list->keys + list->size, 0, (size_t)(list->capacity - list->size) * sizeof(char*));
    }
    
    if (list->types != NULL)
    {
        int8_t *new_types = (int8_t*)realloc(list->types, (size_t)list->capacity * sizeof(int8_t));
        if (!new_types)
        {
            printf("BRUTER_ERROR: failed to reallocate list types\n");
            exit(EXIT_FAILURE);
        }
        list->types = new_types;
        memset(list->types + list->size, 0, (size_t)(list->capacity - list->size) * sizeof(int8_t));
    }
}

STATIC_INLINE void bruter_half(BruterList *list)
{
    BruterValue *new_data = NULL;
    list->capacity /= 2;
    new_data = (BruterValue*)realloc(list->data, (size_t)list->capacity * sizeof(BruterValue));
    if (!new_data)
    {
        printf("BRUTER_ERROR: failed to reallocate list data\n");
        exit(EXIT_FAILURE);
    }
    list->data = new_data;

    if (list->keys != NULL)
    {
        char **new_keys = (char**)realloc(list->keys, (size_t)list->capacity * sizeof(char*));
        if (!new_keys)
        {
            printf("BRUTER_ERROR: failed to reallocate list keys\n");
            exit(EXIT_FAILURE);
        }
        list->keys = new_keys;
    }

    if (list->types != NULL)
    {
        int8_t *new_types = (int8_t*)realloc(list->types, (size_t)list->capacity * sizeof(int8_t));
        if (!new_types)
        {
            printf("BRUTER_ERROR: failed to reallocate list types\n");
            exit(EXIT_FAILURE);
        }
        list->types = new_types;
    }

    if (list->size > list->capacity)
    {
        list->size = list->capacity;
    }
}

STATIC_INLINE void bruter_push(BruterList *list, BruterValue value, const char* key, int8_t type)
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
            size_t len = strlen(key);
            list->keys[list->size] = (char*)malloc(len + 1);
            strcpy(list->keys[list->size], key);
        }
        else 
        {
            list->keys[list->size] = NULL;
        }
    }

    if (list->types != NULL)
    {
        list->types[list->size] = type;
    }
    list->size++;
}

STATIC_INLINE void bruter_push_int(BruterList *list, BruterInt value, const char* key, int8_t type)
{
    bruter_push(list, (BruterValue){.i = value}, key, type);
}

STATIC_INLINE void bruter_push_uint(BruterList *list, BruterUInt value, const char* key, int8_t type)
{
    bruter_push(list, (BruterValue){.u = value}, key, type);
}

STATIC_INLINE void bruter_push_float(BruterList *list, BruterFloat value, const char* key, int8_t type)
{
    bruter_push(list, (BruterValue){.f = value}, key, type);
}

STATIC_INLINE void bruter_push_pointer(BruterList *list, void *value, const char* key, int8_t type)
{
    bruter_push(list, (BruterValue){.p = value}, key, type);
}

STATIC_INLINE void bruter_push_meta(BruterList *list, BruterMeta value)
{
    if (list->size == list->capacity)
    {
        bruter_double(list);
    }
    list->data[list->size] = value.value;

    if (list->keys != NULL)
    {
        if (value.key != NULL)
        {
            list->keys[list->size] = value.key;
        }
        else 
        {
            list->keys[list->size] = NULL;
        }
    }

    if (list->types != NULL)
    {
        list->types[list->size] = value.type;
    }
    list->size++;
}

STATIC_INLINE void bruter_unshift(BruterList *list, BruterValue value, const char* key, int8_t type)
{
    if (list->size == list->capacity)
    {
        bruter_double(list);
    }
    memmove(&(list->data[1]), &(list->data[0]), (size_t)list->size * sizeof(BruterValue));
    list->data[0] = value;
    
    if (list->keys != NULL)
    {
        memmove(&(list->keys[1]), &(list->keys[0]), (size_t)list->size * sizeof(char*));
        if (key != NULL)
        {
            size_t len = strlen(key);
            list->keys[0] = (char*)malloc(len + 1);
            strcpy(list->keys[0], key);
        }
        else 
        {
            list->keys[0] = NULL;
        }
    }

    if (list->types != NULL)
    {
        memmove(&(list->types[1]), &(list->types[0]), (size_t)list->size * sizeof(int8_t));
        list->types[0] = type; // default type is 0
    }

    list->size++;
}

STATIC_INLINE void bruter_unshift_int(BruterList *list, BruterInt value, const char* key, int8_t type)
{
    bruter_unshift(list, (BruterValue){.i = value}, key, type);
}

STATIC_INLINE void bruter_unshift_uint(BruterList *list, BruterUInt value, const char* key, int8_t type)
{
    bruter_unshift(list, (BruterValue){.u = value}, key, type);
}

STATIC_INLINE void bruter_unshift_float(BruterList *list, BruterFloat value, const char* key, int8_t type)
{
    bruter_unshift(list, (BruterValue){.f = value}, key, type);
}

STATIC_INLINE void bruter_unshift_pointer(BruterList *list, void *value, const char* key, int8_t type)
{
    bruter_unshift(list, (BruterValue){.p = value}, key, type);
}

STATIC_INLINE void bruter_unshift_meta(BruterList *list, BruterMeta value)
{
    if (list->size == list->capacity)
    {
        bruter_double(list);
    }
    memmove(&(list->data[1]), &(list->data[0]), (size_t)list->size * sizeof(BruterValue));
    list->data[0] = value.value;

    if (list->keys != NULL)
    {
        memmove(&(list->keys[1]), &(list->keys[0]), (size_t)list->size * sizeof(char*));
        if (value.key != NULL)
        {
            size_t len = strlen(value.key);
            list->keys[0] = (char*)malloc(len + 1);
            strcpy(list->keys[0], value.key);
        }
        else 
        {
            list->keys[0] = NULL;
        }
    }

    if (list->types != NULL)
    {
        memmove(&(list->types[1]), &(list->types[0]), (size_t)list->size * sizeof(int8_t));
        list->types[0] = value.type; // default type is 0
    }

    list->size++;
}

STATIC_INLINE void bruter_insert(BruterList *list, BruterInt i, BruterValue value, const char* key, int8_t type)
{
    if (list->size == list->capacity)
    {
        bruter_double(list);
    }

    if (i < 0 || i > list->size)
    {
        printf("BRUTER_ERROR: index %" PRIdPTR " out of range in list of size %" PRIdPTR " \n", i, list->size);
        exit(EXIT_FAILURE);
    }
    else
    {
        memmove(&(list->data[i + 1]), &(list->data[i]), (size_t)(list->size - i) * sizeof(BruterValue));
        list->data[i] = value;
        if (list->keys != NULL)
        {
            memmove(&(list->keys[i + 1]), &(list->keys[i]), (size_t)(list->size - i) * sizeof(char*));
            if (key != NULL)
            {
                size_t len = strlen(key);
                list->keys[i] = (char*)malloc(len + 1);
                strcpy(list->keys[i], key);
            }
            else 
            {
                list->keys[i] = NULL;
            }
        }

        if (list->types != NULL)
        {
            memmove(&(list->types[i + 1]), &(list->types[i]), (size_t)(list->size - i) * sizeof(int8_t));
            list->types[i] = type;
        }

        list->size++;
    }
}

STATIC_INLINE void bruter_insert_int(BruterList *list, BruterInt i, BruterInt value, const char* key, int8_t type)
{
    bruter_insert(list, i, (BruterValue){.i = value}, key, type);
}

STATIC_INLINE void bruter_insert_uint(BruterList *list, BruterInt i, BruterUInt value, const char* key, int8_t type)
{
    bruter_insert(list, i, (BruterValue){.u = value}, key, type);
}

STATIC_INLINE void bruter_insert_float(BruterList *list, BruterInt i, BruterFloat value, const char* key, int8_t type)
{
    bruter_insert(list, i, (BruterValue){.f = value}, key, type);
}

STATIC_INLINE void bruter_insert_pointer(BruterList *list, BruterInt i, void *value, const char* key, int8_t type)
{
    bruter_insert(list, i, (BruterValue){.p = value}, key, type);
}

STATIC_INLINE void bruter_insert_meta(BruterList *list, BruterInt i, BruterMeta value)
{
    if (list->size == list->capacity)
    {
        bruter_double(list);
    }

    if (i < 0 || i > list->size)
    {
        printf("BRUTER_ERROR: index %" PRIdPTR " out of range in list of size %" PRIdPTR " \n", i, list->size);
        exit(EXIT_FAILURE);
    }
    else
    {
        memmove(&(list->data[i + 1]), &(list->data[i]), (size_t)(list->size - i) * sizeof(BruterValue));
        list->data[i] = value.value;
        if (list->keys != NULL)
        {
            memmove(&(list->keys[i + 1]), &(list->keys[i]), (size_t)(list->size - i) * sizeof(char*));
            if (value.key != NULL)
            {
                size_t len = strlen(value.key);
                list->keys[i] = (char*)malloc(len + 1);
                strcpy(list->keys[i], value.key);
            }
            else 
            {
                list->keys[i] = NULL;
            }
        }

        if (list->types != NULL)
        {
            memmove(&(list->types[i + 1]), &(list->types[i]), (size_t)(list->size - i) * sizeof(int8_t));
            list->types[i] = value.type;
        }

        list->size++;
    }
}

STATIC_INLINE void bruter_define(BruterList *list, BruterValue value, const char* key, int8_t type)
{
    BruterInt index = bruter_find_key(list, key);
    if (index >= 0)
    {
        list->data[index] = value;
        if (list->types != NULL)
        {
            list->types[index] = type;
        }
    }
    else
    {
        bruter_push(list, value, key, type);
    }
}

STATIC_INLINE void bruter_define_int(BruterList *list, BruterInt value, const char* key, int8_t type)
{
    bruter_define(list, (BruterValue){.i = value}, key, type);
}

STATIC_INLINE void bruter_define_uint(BruterList *list, BruterUInt value, const char* key, int8_t type)
{
    bruter_define(list, (BruterValue){.u = value}, key, type);
}

STATIC_INLINE void bruter_define_float(BruterList *list, BruterFloat value, const char* key, int8_t type)
{
    bruter_define(list, (BruterValue){.f = value}, key, type);
}

STATIC_INLINE void bruter_define_pointer(BruterList *list, void *value, const char* key, int8_t type)
{
    bruter_define(list, (BruterValue){.p = value}, key, type);
}

STATIC_INLINE void bruter_define_meta(BruterList *list, BruterMeta value)
{
    BruterInt index = bruter_find_key(list, value.key);
    if (index >= 0)
    {
        list->data[index] = value.value;
        if (list->types != NULL)
        {
            list->types[index] = value.type;
        }
    }
    else
    {
        bruter_push_meta(list, value);
    }
}

#define BRUTER_FUNCTION_CONTENT_POP() \
    BruterValue ret = list->data[list->size - 1]; \
    do { \
        if (list->size == 0)\
        {\
            printf("BRUTER_ERROR: cannot pop from empty list\n");\
            exit(EXIT_FAILURE);\
        }\
\
        if (list->keys != NULL)\
        {\
            free((void*)list->keys[list->size - 1]);\
            list->keys[list->size - 1] = NULL;\
        }\
\
        if (list->types != NULL)\
        {\
            list->types[list->size - 1] = 0; \
        }\
        list->size--; \
    } while(0)

STATIC_INLINE BruterValue bruter_pop(BruterList *list)
{
    BRUTER_FUNCTION_CONTENT_POP();
    return ret;
}

STATIC_INLINE BruterInt bruter_pop_int(BruterList *list)
{
    BRUTER_FUNCTION_CONTENT_POP();
    return ret.i;
}

STATIC_INLINE BruterUInt bruter_pop_uint(BruterList *list)
{
    BRUTER_FUNCTION_CONTENT_POP();
    return ret.u;
}

STATIC_INLINE BruterFloat bruter_pop_float(BruterList *list)
{
    BRUTER_FUNCTION_CONTENT_POP();
    return ret.f;
}

STATIC_INLINE void* bruter_pop_pointer(BruterList *list)
{
    BRUTER_FUNCTION_CONTENT_POP();
    return ret.p;
}

STATIC_INLINE BruterMeta bruter_pop_meta(BruterList *list)
{
    if (list->size == 0)
    {
        printf("BRUTER_ERROR: cannot pop from empty list\n");
        exit(EXIT_FAILURE);
    }
    BruterMeta ret;
    ret.value = list->data[list->size - 1];
    if (list->keys != NULL)
    {
        ret.key = list->keys[list->size - 1];
        list->keys[list->size - 1] = NULL; // set to NULL to avoid double free
    }
    else
    {
        ret.key = NULL;
    }

    if (list->types != NULL)
    {
        ret.type = list->types[list->size - 1];
        list->types[list->size - 1] = 0; // reset type to 0
    }
    else
    {
        ret.type = 0;
    }

    list->size--;
    return ret;
}

#define BRUTER_FUNCTION_CONTENT_SHIFT() \
    BruterValue ret = list->data[0]; \
    do { \
        if (list->size == 0)\
        {\
            printf("BRUTER_ERROR: cannot shift from empty list\n");\
            exit(EXIT_FAILURE);\
        }\
        if (list->size > 1) \
        { \
            memmove(&(list->data[0]), &(list->data[1]), (size_t)(list->size - 1) * sizeof(BruterValue)); \
        } \
        if (list->keys != NULL) \
        { \
            free(list->keys[0]); \
            list->keys[0] = NULL; \
            if (list->size > 1) \
            { \
                memmove(&(list->keys[0]), &(list->keys[1]), (size_t)(list->size - 1) * sizeof(char*)); \
            } \
        } \
        if (list->types != NULL) \
        { \
            list->types[0] = 0; /* reset type to 0 */ \
            if (list->size > 1) \
            { \
                memmove(&(list->types[0]), &(list->types[1]), (size_t)(list->size - 1) * sizeof(int8_t)); \
            } \
        } \
        list->size--; \
    } while(0)

STATIC_INLINE BruterValue bruter_shift(BruterList *list)
{
    BRUTER_FUNCTION_CONTENT_SHIFT();
    return ret;
}

STATIC_INLINE BruterInt bruter_shift_int(BruterList *list)
{
    BRUTER_FUNCTION_CONTENT_SHIFT();
    return ret.i;
}

STATIC_INLINE BruterUInt bruter_shift_uint(BruterList *list)
{
    BRUTER_FUNCTION_CONTENT_SHIFT();
    return ret.u;
}

STATIC_INLINE BruterFloat bruter_shift_float(BruterList *list)
{
    BRUTER_FUNCTION_CONTENT_SHIFT();
    return ret.f;
}

STATIC_INLINE void* bruter_shift_pointer(BruterList *list)
{
    BRUTER_FUNCTION_CONTENT_SHIFT();
    return ret.p;
}

STATIC_INLINE BruterMeta bruter_shift_meta(BruterList *list)
{
    if (list->size == 0)
    {
        printf("BRUTER_ERROR: cannot shift from empty list\n");
        exit(EXIT_FAILURE);
    }
    BruterMeta ret;
    ret.value = list->data[0];
    if (list->keys != NULL)
    {
        ret.key = list->keys[0];
        list->keys[0] = NULL; // set to NULL to avoid double free
    }
    else
    {
        ret.key = NULL;
    }

    if (list->types != NULL)
    {
        ret.type = list->types[0];
        list->types[0] = 0; // reset type to 0
    }
    else
    {
        ret.type = 0;
    }
    
    return ret;
}

#define BRUTER_FUNCTION_CONTENT_REMOVE() \
    BruterValue ret = list->data[i]; \
    do { \
        size_t elements_to_move = 0; \
        if (list->size == 0)\
        {\
            printf("BRUTER_ERROR: cannot pop from empty list\n");\
            exit(EXIT_FAILURE);\
        }\
        elements_to_move = (size_t)(list->size - (i) - 1); \
        \
        /* Move data elements */ \
        memmove(&(list->data[i]), &(list->data[i + 1]), \
                elements_to_move * sizeof(BruterValue)); \
        list->size--; \
        \
        /* Handle keys array if present */ \
        if (list->keys != NULL) { \
            free(list->keys[i]); \
            list->keys[i] = NULL; \
            memmove(&(list->keys[i]), &(list->keys[i + 1]), \
                    elements_to_move * sizeof(char*)); \
        } \
        \
        /* Handle types array if present */ \
        if (list->types != NULL) { \
            list->types[i] = 0; \
            memmove(&(list->types[i]), &(list->types[i + 1]), \
                    elements_to_move * sizeof(int8_t)); \
        } \
    } while(0)

STATIC_INLINE BruterValue bruter_remove(BruterList *list, BruterInt i)
{
    BRUTER_FUNCTION_CONTENT_REMOVE();
    return ret;
}

STATIC_INLINE BruterInt bruter_remove_int(BruterList *list, BruterInt i)
{
    BRUTER_FUNCTION_CONTENT_REMOVE();
    return ret.i;
}

STATIC_INLINE BruterUInt bruter_remove_uint(BruterList *list, BruterInt i)
{
    BRUTER_FUNCTION_CONTENT_REMOVE();
    return ret.u;
}

STATIC_INLINE BruterFloat bruter_remove_float(BruterList *list, BruterInt i)
{
    BRUTER_FUNCTION_CONTENT_REMOVE();
    return ret.f;
}

STATIC_INLINE void* bruter_remove_pointer(BruterList *list, BruterInt i)
{
    BRUTER_FUNCTION_CONTENT_REMOVE();
    return ret.p;
}

STATIC_INLINE BruterMeta bruter_remove_meta(BruterList *list, BruterInt i)
{
    if (list->size == 0)
    {
        printf("BRUTER_ERROR: cannot remove from empty list\n");
        exit(EXIT_FAILURE);
    }
    BruterMeta ret;
    ret.value = list->data[i];
    if (list->keys != NULL)
    {
        ret.key = list->keys[i];
        list->keys[i] = NULL; // set to NULL to avoid double free
    }
    else
    {
        ret.key = NULL;
    }

    if (list->types != NULL)
    {
        ret.type = list->types[i];
        list->types[i] = 0; // reset type to 0
    }
    else
    {
        ret.type = 0;
    }

    return ret;
}

STATIC_INLINE BruterValue bruter_fast_remove(BruterList *list, BruterInt i)
{
    bruter_swap(list, i, list->size - 1);
    return bruter_pop(list);
}

STATIC_INLINE BruterInt bruter_fast_remove_int(BruterList *list, BruterInt i)
{
    bruter_swap(list, i, list->size - 1);
    return bruter_pop_int(list);
}

STATIC_INLINE BruterUInt bruter_fast_remove_uint(BruterList *list, BruterInt i)
{
    bruter_swap(list, i, list->size - 1);
    return bruter_pop_uint(list);
}

STATIC_INLINE BruterFloat bruter_fast_remove_float(BruterList *list, BruterInt i)
{
    bruter_swap(list, i, list->size - 1);
    return bruter_pop_float(list);
}

STATIC_INLINE void* bruter_fast_remove_pointer(BruterList *list, BruterInt i)
{
    bruter_swap(list, i, list->size - 1);
    return bruter_pop_pointer(list);
}

STATIC_INLINE BruterMeta bruter_fast_remove_meta(BruterList *list, BruterInt i)
{
    bruter_swap(list, i, list->size - 1);
    return bruter_pop_meta(list);
}

STATIC_INLINE BruterValue bruter_undefine(BruterList *list, const char* key)
{
    BruterInt index = bruter_find_key(list, key);
    if (index < 0)
    {
        printf("BRUTER_WARNING: bruter_undefine called with key '%s' not found in list\n", key);
    }
    return bruter_remove(list, index);
}

STATIC_INLINE BruterInt bruter_undefine_int(BruterList *list, const char* key)
{
    BruterInt index = bruter_find_key(list, key);
    if (index < 0)
    {
        printf("BRUTER_WARNING: bruter_undefine_int called with key '%s' not found in list\n", key);
    }
    return bruter_remove_int(list, index);
}

STATIC_INLINE BruterUInt bruter_undefine_uint(BruterList *list, const char* key)
{
    BruterInt index = bruter_find_key(list, key);
    if (index < 0)
    {
        printf("BRUTER_WARNING: bruter_undefine_uint called with key '%s' not found in list\n", key);
    }
    return bruter_remove_uint(list, index);
}

STATIC_INLINE BruterFloat bruter_undefine_float(BruterList *list, const char* key)
{
    BruterInt index = bruter_find_key(list, key);
    if (index < 0)
    {
        printf("BRUTER_WARNING: bruter_undefine_float called with key '%s' not found in list\n", key);
    }
    return bruter_remove_float(list, index);
}

STATIC_INLINE void* bruter_undefine_pointer(BruterList *list, const char* key)
{
    BruterInt index = bruter_find_key(list, key);
    if (index < 0)
    {
        printf("BRUTER_WARNING: bruter_undefine_pointer called with key '%s' not found in list\n", key);
    }
    return bruter_remove_pointer(list, index);
}

STATIC_INLINE BruterMeta bruter_undefine_meta(BruterList *list, const char* key)
{
    BruterInt index = bruter_find_key(list, key);
    if (index < 0)
    {
        printf("BRUTER_WARNING: bruter_undefine_meta called with key '%s' not found in list\n", key);
    }
    return bruter_remove_meta(list, index);
}

STATIC_INLINE void bruter_swap(BruterList *list, BruterInt i1, BruterInt i2)
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

    if (list->types != NULL)
    {
        int8_t tmp_type = list->types[i1];
        list->types[i1] = list->types[i2];
        list->types[i2] = tmp_type;
    }
}

STATIC_INLINE BruterInt bruter_find(const BruterList *list, BruterValue value) 
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

STATIC_INLINE BruterInt bruter_find_int(const BruterList *list, BruterInt value)
{
    return bruter_find(list, (BruterValue){.i = value});
}

STATIC_INLINE BruterInt bruter_find_uint(const BruterList *list, BruterUInt value)
{
    return bruter_find(list, (BruterValue){.u = value});
}

STATIC_INLINE BruterInt bruter_find_float(const BruterList *list, BruterFloat value)
{
    return bruter_find(list, (BruterValue){.f = value});
}

STATIC_INLINE BruterInt bruter_find_pointer(const BruterList *list, void *value)
{
    return bruter_find(list, (BruterValue){.p = value});
}

STATIC_INLINE BruterInt bruter_find_meta(const BruterList *list, BruterMeta value)
{
    for (BruterInt i = 0; i < list->size; i++)
    {
        if (list->data[i].i == value.value.i && 
            ((list->keys == NULL && value.key == NULL) || 
             (list->keys != NULL && value.key != NULL && strcmp(list->keys[i], value.key) == 0)) &&
            (list->types == NULL || list->types[i] == value.type))
        {
            return i;
        }
    }

    return -1;
}

STATIC_INLINE BruterInt bruter_find_key(const BruterList *list, const char* key)
{
    if (list->keys == NULL)
    {
        printf("BRUTER_ERROR: bruter_find_key called on a list that is not a table\n");
        exit(EXIT_FAILURE);
    }

    for (BruterInt i = 0; i < list->size; i++)
    {
        if (list->keys[i] != NULL)
        {
            if (strcmp(list->keys[i], key) == 0)
            {
                return i;
            }
        }
    }

    return -1;
}

STATIC_INLINE void bruter_reverse(BruterList *list)
{
    for (BruterInt i = 0; i < list->size / 2; i++)
    {
        bruter_swap(list, i, list->size - i - 1);
    }
}

STATIC_INLINE BruterList* bruter_copy(const BruterList *list)
{
    BruterList *copy = bruter_new(list->capacity, list->keys != NULL, list->types != NULL);
    
    if (copy == NULL)
    {
        printf("BRUTER_ERROR: failed to allocate memory for BruterList copy\n");
        exit(EXIT_FAILURE);
    }
    copy->size = list->size;
    memcpy(copy->data, list->data, (size_t)copy->size * sizeof(BruterValue));
    
    if (list->keys != NULL)
    {
        for (BruterInt i = 0; i < copy->size; i++)
        {
            if (list->keys[i] != NULL)
            {
                size_t len = strlen(list->keys[i]);
                strcpy(copy->keys[i], list->keys[i]);
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
    
    if (list->types != NULL)
    {
        if (copy->types == NULL)
        {
            printf("BRUTER_ERROR: failed to allocate memory for BruterList types copy\n");
            exit(EXIT_FAILURE);
        }
        memcpy(copy->types, list->types, (size_t)copy->size * sizeof(int8_t));
    }
    else
    {
        copy->types = NULL;
    }

    
    copy->capacity = list->capacity;

    return copy;
}

STATIC_INLINE BruterValue bruter_get(const BruterList *list, BruterInt i)
{
    if (i < 0 || i >= list->size)
    {
        return (BruterValue){.i=-1}; // return -1 if index is out of range
    }
    return list->data[i];
}

STATIC_INLINE BruterInt bruter_get_int(const BruterList *list, BruterInt i)
{
    if (i < 0 || i >= list->size)
    {
        return -1; // return -1 if index is out of range
    }
    return list->data[i].i;
}

STATIC_INLINE BruterUInt bruter_get_uint(const BruterList *list, BruterInt i)
{
    if (i < 0 || i >= list->size)
    {
        return 0; // return 0 if index is out of range
    }
    return list->data[i].u;
}

STATIC_INLINE BruterFloat bruter_get_float(const BruterList *list, BruterInt i)
{
    if (i < 0 || i >= list->size)
    {
        return (BruterFloat)0.0; // return 0.0 if index is out of range
    }
    return list->data[i].f;
}

STATIC_INLINE void* bruter_get_pointer(const BruterList *list, BruterInt i)
{
    if (i < 0 || i >= list->size)
    {
        return NULL; // return NULL if index is out of range
    }
    return list->data[i].p;
}

STATIC_INLINE BruterMeta bruter_get_meta(const BruterList *list, BruterInt i)
{
    if (i < 0 || i >= list->size)
    {
        return (BruterMeta){.value = {.i = -1}, .key = NULL, .type = 0}; // return a default value if index is out of range
    }
    
    BruterMeta ret;
    ret.value = list->data[i];
    
    if (list->keys != NULL)
    {
        ret.key = list->keys[i];
    }
    else
    {
        ret.key = NULL;
    }

    if (list->types != NULL)
    {
        ret.type = list->types[i];
    }
    else
    {
        ret.type = 0; // default type is 0
    }

    return ret;
}

STATIC_INLINE void bruter_set(BruterList *list, BruterInt i, BruterValue value, const char* key, int8_t type)
{
    if (i < 0 || i >= list->size)
    {
        printf("BRUTER_ERROR: index %" PRIdPTR " out of range in list of size %" PRIdPTR " \n", i, list->size);
        exit(EXIT_FAILURE);
    }
    list->data[i] = value;
    if (list->keys != NULL)
    {
        if (key != NULL)
        {
            size_t len = strlen(key);
            if (list->keys[i] != NULL)
            {
                free(list->keys[i]);
            }
            list->keys[i] = (char*)malloc(len + 1);
            strcpy(list->keys[i], key);
        }
        else 
        {
            if (list->keys[i] != NULL)
            {
                free(list->keys[i]);
                list->keys[i] = NULL;
            }
        }
    }
    if (list->types != NULL)
    {
        list->types[i] = type;
    }
}

STATIC_INLINE void bruter_set_int(BruterList *list, BruterInt i, BruterInt value, const char* key, int8_t type)
{
    bruter_set(list, i, (BruterValue){.i = value}, key, type);
}

STATIC_INLINE void bruter_set_uint(BruterList *list, BruterInt i, BruterUInt value, const char* key, int8_t type)
{
    bruter_set(list, i, (BruterValue){.u = value}, key, type);
}

STATIC_INLINE void bruter_set_float(BruterList *list, BruterInt i, BruterFloat value, const char* key, int8_t type)
{
    bruter_set(list, i, (BruterValue){.f = value}, key, type);
}

STATIC_INLINE void bruter_set_pointer(BruterList *list, BruterInt i, void *value, const char* key, int8_t type)
{
    bruter_set(list, i, (BruterValue){.p = value}, key, type);
}

STATIC_INLINE void bruter_set_meta(BruterList *list, BruterInt i, BruterMeta value)
{
    if (i < 0 || i >= list->size)
    {
        printf("BRUTER_ERROR: index %" PRIdPTR " out of range in list of size %" PRIdPTR " \n", i, list->size);
        exit(EXIT_FAILURE);
    }
    list->data[i] = value.value;

    if (list->keys != NULL)
    {
        if (value.key != NULL)
        {
            size_t len = strlen(value.key);
            if (list->keys[i] != NULL)
            {
                free(list->keys[i]);
            }
            list->keys[i] = (char*)malloc(len + 1);
            strcpy(list->keys[i], value.key);
        }
        else 
        {
            if (list->keys[i] != NULL)
            {
                free(list->keys[i]);
                list->keys[i] = NULL;
            }
        }
    }

    if (list->types != NULL)
    {
        list->types[i] = value.type;
    }
}

STATIC_INLINE char* bruter_get_key(const BruterList *list, BruterInt i)
{
    if (list->keys == NULL)
    {
        printf("BRUTER_ERROR: bruter_get_key called on a list that is not a table\n");
        exit(EXIT_FAILURE);
    }
    
    if (i < 0 || i >= list->size)
    {
        printf("BRUTER_ERROR: index %" PRIdPTR " out of range in list of size %" PRIdPTR " \n", i, list->size);
        return NULL;
    }
    
    return list->keys[i];
}

STATIC_INLINE void bruter_set_key(const BruterList *list, BruterInt i, const char *key)
{
    if (list->keys == NULL)
    {
        printf("BRUTER_ERROR: bruter_set_key called on a list that is not a table\n");
        exit(EXIT_FAILURE);
    }
    
    if (i < 0 || i >= list->size)
    {
        printf("BRUTER_ERROR: index %" PRIdPTR " out of range in list of size %" PRIdPTR " \n", i, list->size);
        exit(EXIT_FAILURE);
    }
    
    if (list->keys[i] != NULL)
    {
        free(list->keys[i]);
    }
    
    if (key != NULL)
    {
        size_t len = strlen(key);
        list->keys[i] = (char*)malloc(len + 1);
        strcpy(list->keys[i], key);
    }
    else 
    {
        list->keys[i] = NULL;
    }
}

STATIC_INLINE int8_t bruter_get_type(const BruterList *list, BruterInt i)
{
    if (list->types == NULL)
    {
        printf("BRUTER_ERROR: bruter_get_type called on a list that is not typed\n");
        exit(EXIT_FAILURE);
    }
    
    if (i < 0 || i >= list->size)
    {
        printf("BRUTER_ERROR: index %" PRIdPTR " out of range in list of size %" PRIdPTR " \n", i, list->size);
        return 0; // return 0 if index is out of range
    }
    
    return list->types[i];
}

STATIC_INLINE void bruter_set_type(const BruterList *list, BruterInt i, int8_t type)
{
    if (list->types == NULL)
    {
        printf("BRUTER_ERROR: bruter_set_type called on a list that is not typed\n");
        exit(EXIT_FAILURE);
    }
    
    if (i < 0 || i >= list->size)
    {
        printf("BRUTER_ERROR: index %" PRIdPTR " out of range in list of size %" PRIdPTR " \n", i, list->size);
        exit(EXIT_FAILURE);
    }
    
    list->types[i] = type;
}

STATIC_INLINE const char* bruter_get_version(void)
{
    return BRUTER_VERSION;
}

STATIC_INLINE void* bruter_alloc(BruterList* arena, size_t size)
{
    size_t aligned_size = (size + sizeof(void*) - 1) & ~(sizeof(void*) - 1);
    // we use arena->size as offset
    if (arena->size + aligned_size > arena->capacity)
    {
        bruter_double(arena);
    }
    void *ptr = arena->data + arena->size;
    arena->size += aligned_size;
    return ptr;
}

// if you want to return something, pass a stack, values will be there
// if you do not provide a stack, a new one will be created and freed at the end
STATIC_INLINE void bruter_interpret(BruterList *context, const char* input_str, BruterList* _splited, BruterList* _stack)
{
    BruterList *splited;
    BruterList *stack;
    char* original_str = NULL;
    if (_splited == NULL)
    {
        splited = bruter_new(BRUTER_DEFAULT_SIZE, false, true);
        original_str = strdup(input_str); // Duplicate the input string to avoid modifying the original
        char* token = strtok(original_str, "\n\t\r ");
        while (token != NULL)
        {
            bruter_push_pointer(splited, token, NULL, BRUTER_TYPE_NULL);
            token = strtok(NULL, "\n\t\r ");
        }
    }
    else
    {
        splited = _splited;
    }

    if (_stack == NULL)
    {
        stack = bruter_new(BRUTER_DEFAULT_SIZE, false, true);
    }
    else
    {
        stack = _stack;
    }

    for (BruterInt i = 0; i < splited->size; i++)
    {
        char* token = (char*)splited->data[i].p;
        uint8_t token_type = splited->types[i];

        // we assume its already processed if its type is not BRUTER_TYPE_NULL
        if (token_type != BRUTER_TYPE_NULL)
        {
            bruter_push_meta(stack, (BruterMeta){.value = splited->data[i], .key = NULL, .type = token_type});
            continue;
        }
        else if (token == NULL || token[0] == '\0') continue; // Skip empty tokens
        
        switch(token[0])
        {
            case '!': // call stack
            {
                switch (stack->types[stack->size - 1])
                {
                    case BRUTER_TYPE_FUNCTION:
                        Function func = (Function)bruter_pop_pointer(stack);
                        func(stack);
                        break;
                    case BRUTER_TYPE_LIST:
                    {
                        BruterList* func_list = (BruterList*)bruter_copy(bruter_pop_pointer(stack));
                        BruterInt arg_count = stack->size;
                        for (BruterInt j = 0; j < func_list->size; j++)
                        {
                            if (((char*)func_list->data[j].p)[0] == '$')
                            {
                                BruterInt index = strtol(((char*)func_list->data[j].p) + 1, NULL, 10);
                                if (index >= arg_count)
                                {
                                    arg_count = index + 1;
                                }
                                bruter_set_meta(func_list, j, bruter_get_meta(stack, stack->size - index - 1));
                            }
                        }
                        for (BruterInt j = 0; j < arg_count; j++)
                        {
                            bruter_pop(stack); // remove used arguments from stack
                        }
                        bruter_interpret(context, NULL, func_list, stack);
                        bruter_free(func_list);
                        break;
                    }
                    default:
                        printf("BRUTER_ERROR: trying to call a non-function value from stack\n");
                        exit(EXIT_FAILURE);
                }
            }
            break;
            case '&': // stack
            {
                // lets store the stack in the code, so we dont need to come here again and again
                splited->data[i].p = stack; // store the stack in splited
                splited->types[i] = BRUTER_TYPE_LIST; // change the type to list

                bruter_push_pointer(stack, stack, NULL, BRUTER_TYPE_LIST);
            }
            break;
            case '@': // context
            {
                // lets store the context in the code, so we dont need to come here again and again
                splited->data[i].p = context; // store the context in splited
                splited->types[i] = BRUTER_TYPE_LIST; // change the type to list

                bruter_push_pointer(stack, context, NULL, BRUTER_TYPE_LIST);
            }
            break;
            case '%': // the code itself, the splited string
            {
                // lets store the splited in the code, so we dont need to come here again and again
                splited->data[i].p = splited; // store the splited in splited
                splited->types[i] = BRUTER_TYPE_LIST; // change the type to list
                bruter_push_pointer(stack, splited, NULL, BRUTER_TYPE_LIST);
            }
            break;
            case '?':
            {
                BruterInt condition = bruter_pop_int(stack);
                BruterInt iftrue_position = bruter_pop_int(stack);
                if (condition)
                {
                    i = iftrue_position - 1;
                }
            }
            break;
            case '.': 
            case '-':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9': // number
            {
                if (strchr(token, '.')) // float
                {
                    BruterFloat value;
                    if (sizeof(void*) == 8) // double precision
                    {
                        value = strtod(token, NULL);
                    }
                    else // single precision
                    {
                        value = strtof(token, NULL);
                    }
                    bruter_push_float(stack, value, NULL, BRUTER_TYPE_FLOAT);
                    splited->data[i].f = value; // store the value as float
                    splited->types[i] = BRUTER_TYPE_FLOAT; // change the type to float
                }
                else // int
                {
                    unsigned long value = strtoul(token, NULL, 10);
                
                    bruter_push_int(stack, value, NULL, BRUTER_TYPE_ANY);
                    splited->data[i].u = value; // store the value as uint
                    splited->types[i] = BRUTER_TYPE_ANY; // change the type to int
                }
            }
            break;
            case ',': // string
            {
                char* str = token + 1;
                for (int j = 0; str[j] != '\0'; j++)
                {
                    if (str[j] == 26) str[j] = '\n';
                    else if (str[j] == 29) str[j] = '\t';
                    else if (str[j] == 30) str[j] = ' ';

                    // deal with escaped characters
                    else if (str[j] == '\\' && str[j + 1] != '\0')
                    {
                        memmove(&str[j], &str[j + 1], strlen(str) - j); // remove the backslash
                        switch (str[j])
                        {
                            case 'n': str[j] = '\n'; break;
                            case 't': str[j] = '\t'; break;
                            case '\\': str[j] = '\\'; break; // keep the backslash
                            case '\'': str[j] = '\''; break; // keep the single quote
                            case '\"': str[j] = '\"'; break; // keep the double quote
                            default: // any other character after backslash is treated as normal character
                                break;
                        }
                    }
                }
                
                splited->data[i].p = str;
                splited->types[i] = BRUTER_TYPE_BUFFER;

                bruter_push_pointer(stack, str, NULL, BRUTER_TYPE_BUFFER);
            }
            break;
            case ':': // runtime label 
            {
                BruterInt label_position = i;
                // we remove the label from the program
                char* label_str = (char*)bruter_remove_pointer(splited, i);
                bruter_push_int(context, label_position, label_str + 1, BRUTER_TYPE_ANY);
                i--;
            }
            break;
            case '$': // runtime function arg
            {
                BruterInt arg_index = (BruterInt)strtol(token + 1, NULL, 10);
                if (arg_index < 0)
                {
                    arg_index += (BruterInt)stack->size;
                }
                if (arg_index < 0 || arg_index >= stack->size)
                {
                    printf("BRUTER_ERROR: argument index %" PRIdPTR " out of range in stack of size %" PRIdPTR " \n", arg_index, stack->size);
                    exit(EXIT_FAILURE);
                }
                
                bruter_push_int(stack, arg_index, NULL, BRUTER_TYPE_NULL);
            }
            break;
            default:
            {
                BruterInt found = -1;
                if (token[0] == '#') // static values are not re-evaluated when encountered again
                {
                    if (isdigit(token[1]))
                    {
                        found = (BruterInt)strtol(token + 1, NULL, 10);
                        if (found < 0)
                        {
                            found += (BruterInt)context->size;
                        }
                        splited->data[i].u = context->data[found].u;
                        splited->types[i] = context->types[found];
                    }
                    else
                    {
                        found = bruter_find_key(context, token + 1);
                        if (found >= 0)
                        {
                            splited->data[i].u = context->data[found].u;
                            splited->types[i] = context->types[found];
                        }
                        else
                        {
                            printf("WARNING: Variable '%s' not found in context\n", token);
                        }
                    }
                    bruter_push_meta(stack, bruter_get_meta(context, found));

                    continue; // skip pushing to stack again
                }

                found = bruter_find_key(context, token);
                if (found != -1)
                {
                    BruterMeta meta = bruter_get_meta(context, found);
                    meta.key = NULL; // we don't need the key here
                    bruter_push_meta(stack, meta);
                }
                else
                {
                    // If not found
                    printf("WARNING: Variable '%s' not found in context\n", token);
                    // We can still push a null value to the stack
                    bruter_push_int(stack, 0, NULL, BRUTER_TYPE_NULL);
                }
            }
            break;
        }
    }
    if (_splited == NULL) 
        bruter_free(splited); // free splited only if it was created here
    if (_stack == NULL) 
        bruter_free(stack); // free stack only if it was created here

    free(original_str); // free the original string
    return stack;
}

#endif // ifndef BRUTER_AS_HEADER macro
#endif // ifndef BRUTER_H macro