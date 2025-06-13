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
// if you are under a pure C99 eviroment, beware of the use of the union,
// C99 defines that the used type is always the last it was defined to,
// you can use memcpy to "pun" the type easily, although most modern compilers doesnt care about this specific union behavior
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
    BruterInt capacity;
    BruterInt size;

    BruterValue *data;
    
    #ifndef BRUTER_KEYLESS
    char **keys;
    #endif

    #ifndef BRUTER_TYPELESS
    int8_t *types;
    #endif

} BruterList;

#ifdef USE_SHORT_TYPES

#warning "USE_SHORT_TYPES is defined; using short type aliases like Int, Float, etc."

#define Int BruterInt
#define UInt BruterUInt
#define Float BruterFloat
#define Value BruterValue
#define List BruterList

#endif

static inline BruterValue _bruter_value_i(BruterInt value);
static inline BruterValue _bruter_value_u(BruterUInt value);
static inline BruterValue _bruter_value_f(BruterFloat value);
static inline BruterValue _bruter_value_p(void *value);

// BruterList functions(also work for tables as tables are just lists with keys)
// create a new list with the given size, if size is 0, it will be initialized with NULL data and then allocated when needed
static inline BruterList   *_bruter_init(BruterInt size, bool is_table, bool is_typed);
// free the list 
static inline void          _bruter_free(BruterList *list);
// double the list capacity
static inline void          _bruter_double(BruterList *list);
// halve the list capacity
static inline void          _bruter_half(BruterList *list);
// push a value to the end of the list
static inline void          _bruter_push(BruterList *list, BruterValue value, const char* key, int8_t type);
// unshift a value to the start of the list
static inline void          _bruter_unshift(BruterList *list, BruterValue value, const char* key, int8_t type);
// insert a value at index i in the list
static inline void          _bruter_insert(BruterList *list, BruterInt i, BruterValue value, const char* key, int8_t type);
// pop a value from the end of the list
static inline BruterValue   _bruter_pop(BruterList *list);
// shift a value from the start of the list
static inline BruterValue   _bruter_shift(BruterList *list);
// remove and return the value at index i in the list, shifting the rest of the list
static inline BruterValue   _bruter_remove(BruterList *list, BruterInt i);
// fast remove a value at index i in the list, swapping it with the last element and popping it
static inline BruterValue   _bruter_fast_remove(BruterList *list, BruterInt i);
// swap two values in the list at indices i1 and i2
static inline void          _bruter_swap(BruterList *list, BruterInt i1, BruterInt i2);
// find the index of a value in the list, returns -1 if not found
static inline BruterInt     _bruter_find(BruterList *list, BruterValue value, const char* key);
// reverse the list in place
static inline void          _bruter_reverse(BruterList *list);
// create a copy of the list, with the same capacity and size, but new data array
static inline BruterList   *_bruter_copy(BruterList *list);
// concatenate two lists, appending src to dest, resizing dest if necessary
static inline void          _bruter_concat(BruterList *dest, BruterList *src);
// if context is NULL, it will call direcly from list->data[0].p and the result itself
// if context is not NULL, it will call from context->data[list->data[0].i].p and return the index of the result in context
// if context is not NULL, the result will be always an int, because it return the index of the result in context
static inline BruterValue   _bruter_call(BruterList *context, BruterList *list);
// get a value at index i in the list, returns a value with i set to -1 if index is out of range
static inline BruterValue   _bruter_get(BruterList *list, BruterInt i);
// set a value at index i in the list, if index is out of range, it will print an error and exit
static inline void          _bruter_set(BruterList *list, BruterInt i, BruterValue value);

#ifndef BRUTER_KEYLESS
// get a key at index i in the list, returns NULL if index is out of range or if the list is not a table
static inline char*         _bruter_get_key(BruterList *list, BruterInt i);
// set a key at index i in the list, if index is out of range, it will print an error and exit
static inline void          _bruter_set_key(BruterList *list, BruterInt i , const char *key);
#endif

#ifndef BRUTER_TYPELESS
// get a type at index i in the list, returns 0 if index is out of range or if the list is not typed
static inline int8_t        _bruter_get_type(BruterList *list, BruterInt i);
// set a type at index i in the list, if index is out of range, it will print an error and exit
static inline void          _bruter_set_type(BruterList *list, BruterInt i, int8_t type);
#endif

// MACRO IMPLEMENTATIONS

// under this section, every macro have a specific implementation for each case
#if defined(BRUTER_TYPELESS) && defined(BRUTER_KEYLESS)
    #define bruter_init(size) _bruter_init(size, false, false)
    #define bruter_push(list, value) _bruter_push(list, value, NULL, 0)
    #define bruter_unshift(list, value) _bruter_unshift(list, value, NULL, 0)
    #define bruter_insert(list, i, value) _bruter_insert(list, i, value, NULL, 0)
#elif defined(BRUTER_TYPELESS)
    #define bruter_init(size, is_table) _bruter_init(size, is_table, false)
    #define bruter_push(list, value, key) _bruter_push(list, value, key, 0)
    #define bruter_unshift(list, value, key) _bruter_unshift(list, value, key, 0)
    #define bruter_insert(list, i, value, key) _bruter_insert(list, i, value, key, 0)
#elif defined(BRUTER_KEYLESS)
    #define bruter_init(size, is_key) _bruter_init(size, false, is_key)
    #define bruter_push(list, value, key) _bruter_push(list, value, NULL, 0)
    #define bruter_unshift(list, value, key) _bruter_unshift(list, value, NULL, 0)
    #define bruter_insert(list, i, value, key) _bruter_insert(list, i, value, NULL, 0)
#else
    #define bruter_init(size, is_table, is_typed) _bruter_init(size, is_table, is_typed)
    #define bruter_push(list, value, key, type) _bruter_push(list, value, key, type)
    #define bruter_unshift(list, value, key, type) _bruter_unshift(list, value, key, type)
    #define bruter_insert(list, i, value, key, type) _bruter_insert(list, i, value, key, type)
#endif

// under this section macros are only affected by keyless
#if defined(BRUTER_KEYLESS)
    #define bruter_find(list, value) _bruter_find(list, value, NULL)
#else
    #define bruter_get_key(list, i) _bruter_get_key(list, i)
    #define bruter_set_key(list, i, key) _bruter_set_key(list, i, key)
    #define bruter_find(list, value, key) _bruter_find(list, value, key)
#endif

// under this section macros are only affected by typeless
#if defined(BRUTER_TYPELESS)
    #define bruter_get_type(list, i) _bruter_get_type(list, i)
    #define bruter_set_type(list, i, type) _bruter_set_type(list, i, type)
#endif

// these macros are always the same no matter the implementation
#define bruter_free(list) _bruter_free(list)
#define bruter_double(list) _bruter_double(list)
#define bruter_half(list) _bruter_half(list)
#define bruter_pop(list) _bruter_pop(list)
#define bruter_shift(list) _bruter_shift(list)
#define bruter_remove(list, i) _bruter_remove(list, i)
#define bruter_fast_remove(list, i) _bruter_fast_remove(list, i)
#define bruter_swap(list, i1, i2) _bruter_swap(list, i1, i2)
#define bruter_reverse(list) _bruter_reverse(list)
#define bruter_copy(list) _bruter_copy(list)
#define bruter_concat(dest, src) _bruter_concat(dest, src)
#define bruter_call(context, list) _bruter_call(context, list)
#define bruter_get(list, i) _bruter_get(list, i)
#define bruter_set(list, i, value) _bruter_set(list, i, value)
#define bruter_value_i(value) _bruter_value_i(value)
#define bruter_value_u(value) _bruter_value_u(value)
#define bruter_value_f(value) _bruter_value_f(value)
#define bruter_value_p(value) _bruter_value_p(value)

// functions implementations
// functions implementations
// functions implementations
// functions implementations

static inline BruterValue _bruter_value_i(BruterInt value)
{
    return (BruterValue){.i = value};
}

static inline BruterValue _bruter_value_u(BruterUInt value)
{
    return (BruterValue){.u = value};
}

static inline BruterValue _bruter_value_f(BruterFloat value)
{
    return (BruterValue){.f = value};
}

static inline BruterValue _bruter_value_p(void *value)
{
    return (BruterValue){.p = value};
}

static inline BruterList *_bruter_init(BruterInt size, bool is_table, bool is_typed)
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

#ifndef BRUTER_KEYLESS
    if (is_table)
    {
        list->keys = (char**)calloc(size, sizeof(char*)); // we need all keys to be NULL
        if (list->keys == NULL)
        {
            printf("BRUTER_ERROR: failed to allocate memory for BruterList keys\n");
            free(list->data);
            free(list);
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        list->keys = NULL;
    }
#endif

#ifndef BRUTER_TYPELESS
    if (is_typed)
    {
        list->types = (int8_t*)calloc(size, sizeof(int8_t)); // we need all types to be 0
        if (list->types == NULL)
        {
            printf("BRUTER_ERROR: failed to allocate memory for BruterList types\n");
            free(list->data);
            if (list->keys != NULL) free(list->keys);
            free(list);
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        list->types = NULL;
    }
#endif

    return list;
}

static inline void _bruter_free(BruterList *list)
{
#ifndef BRUTER_KEYLESS
    if (list->keys != NULL)
    {
        for (BruterInt i = 0; i < list->size; i++)
        {
            free(list->keys[i]);
        }

        free(list->keys);
    }
#endif

#ifndef BRUTER_TYPELESS
        free(list->types);
#endif
    free(list->data);
    free(list);
}

static inline void _bruter_double(BruterList *list)
{
    list->capacity = list->capacity == 0 ? 1 : list->capacity * 2;
    BruterValue *new_data = realloc(list->data, list->capacity * sizeof(BruterValue));
    if (!new_data)
    {
        printf("BRUTER_ERROR: failed to reallocate list data\n");
        exit(EXIT_FAILURE);
    }
    list->data = new_data;

#ifndef BRUTER_KEYLESS
    if (list->keys != NULL)
    {
        char **new_keys = realloc(list->keys, list->capacity * sizeof(char*));
        if (!new_keys)
        {
            printf("BRUTER_ERROR: failed to reallocate list keys\n");
            exit(EXIT_FAILURE);
        }
        list->keys = new_keys;
        memset(list->keys + list->size, 0, (list->capacity - list->size) * sizeof(char*));
    }
#endif

#ifndef BRUTER_TYPELESS
    if (list->types != NULL)
    {
        int8_t *new_types = realloc(list->types, list->capacity * sizeof(int8_t));
        if (!new_types)
        {
            printf("BRUTER_ERROR: failed to reallocate list types\n");
            exit(EXIT_FAILURE);
        }
        list->types = new_types;
        memset(list->types + list->size, 0, (list->capacity - list->size) * sizeof(int8_t));
    }
#endif
}

static inline void _bruter_half(BruterList *list)
{
    list->capacity /= 2;

    BruterValue *new_data = realloc(list->data, list->capacity * sizeof(BruterValue));
    if (!new_data)
    {
        printf("BRUTER_ERROR: failed to reallocate list data\n");
        exit(EXIT_FAILURE);
    }
    list->data = new_data;

#ifndef BRUTER_KEYLESS
    if (list->keys != NULL)
    {
        char **new_keys = realloc(list->keys, list->capacity * sizeof(char*));
        if (!new_keys)
        {
            printf("BRUTER_ERROR: failed to reallocate list keys\n");
            exit(EXIT_FAILURE);
        }
        list->keys = new_keys;
    }
#endif

#ifndef BRUTER_TYPELESS
    if (list->types != NULL)
    {
        int8_t *new_types = realloc(list->types, list->capacity * sizeof(int8_t));
        if (!new_types)
        {
            printf("BRUTER_ERROR: failed to reallocate list types\n");
            exit(EXIT_FAILURE);
        }
        list->types = new_types;
    }
#endif

    if (list->size > list->capacity)
    {
        list->size = list->capacity;
    }
}

static inline void _bruter_push(BruterList *list, BruterValue value, const char* key, int8_t type)
{
    if (list->size == list->capacity)
    {
        _bruter_double(list);
    }
    list->data[list->size] = value;

#ifndef BRUTER_KEYLESS
    if (list->keys != NULL)
    {
        if (key != NULL)
        {
            int len = strlen(key);
            list->keys[list->size] = malloc(len + 1);
            strcpy(list->keys[list->size], key);
        }
        else 
        {
            list->keys[list->size] = NULL;
        }
    }
#endif

#ifndef BRUTER_TYPELESS
    if (list->types != NULL)
    {
        list->types[list->size] = 0; // default type is 0
    }
#endif
    list->size++;
}

static inline void _bruter_unshift(BruterList *list, BruterValue value, const char* key, int8_t type)
{
    if (list->size == list->capacity)
    {
        _bruter_double(list);
    }
    memmove(&(list->data[1]), &(list->data[0]), list->size * sizeof(BruterValue));
    list->data[0] = value;
    
#ifndef BRUTER_KEYLESS
    if (list->keys != NULL)
    {
        memmove(&(list->keys[1]), &(list->keys[0]), list->size * sizeof(char*));
        if (key != NULL)
        {
            int len = strlen(key);
            list->keys[0] = malloc(len + 1);
            strcpy(list->keys[0], key);
        }
        else 
        {
            list->keys[0] = NULL;
        }
    }
#endif

#ifndef BRUTER_TYPELESS
    if (list->types != NULL)
    {
        memmove(&(list->types[1]), &(list->types[0]), list->size * sizeof(int8_t));
        list->types[0] = 0; // default type is 0
    }
#endif

    list->size++;
}

static inline void _bruter_insert(BruterList *list, BruterInt i, BruterValue value, const char* key, int8_t type)
{
    if (list->size == list->capacity)
    {
        _bruter_double(list);
    }

    if (i <= list->size) 
    {
        memmove(&(list->data[i + 1]), &(list->data[i]), (list->size - i) * sizeof(BruterValue));
        list->data[i] = value;

#ifndef BRUTER_KEYLESS
        if (list->keys != NULL)
        {
            memmove(&(list->keys[i + 1]), &(list->keys[i]), (list->size - i) * sizeof(char*));
            if (key != NULL)
            {
                int len = strlen(key);
                list->keys[i] = malloc(len + 1);
                strcpy(list->keys[i], key);
            }
            else 
            {
                list->keys[i] = NULL;
            }
        }
#endif

#ifndef BRUTER_TYPELESS
        if (list->types != NULL)
        {
            memmove(&(list->types[i + 1]), &(list->types[i]), (list->size - i) * sizeof(int8_t));
            list->types[i] = 0; // default type is 0
        }
#endif

        list->size++;
    } 
    else 
    {
        printf("BRUTER_ERROR: index %" PRIdPTR " out of range in list of size %" PRIdPTR " \n", i, list->size);
        exit(EXIT_FAILURE);
    }
}

static inline BruterValue _bruter_pop(BruterList *list)
{
    if (list->size == 0)
    {
        printf("BRUTER_ERROR: cannot pop from empty list\n");
        exit(EXIT_FAILURE);
    }

    #ifndef BRUTER_KEYLESS
        if (list->keys != NULL)
        {
            free(list->keys[list->size - 1]);
            list->keys[list->size - 1] = NULL;
        }
    #endif
    #ifndef BRUTER_TYPELESS
        if (list->types != NULL)
        {
            list->types[list->size - 1] = 0; // reset type to 0
        }
    #endif

    return list->data[--list->size];
}

static inline BruterValue _bruter_shift(BruterList *list)
{
    BruterValue ret = list->data[0];
    if (list->size > 1) 
    {
        memmove(&(list->data[0]), &(list->data[1]), (list->size - 1) * sizeof(BruterValue));
    }

    #ifndef BRUTER_KEYLESS
        if (list->keys != NULL)
        {
            free(list->keys[0]);
            list->keys[0] = NULL;
            if (list->size > 1) 
            {
                memmove(&(list->keys[0]), &(list->keys[1]), (list->size - 1) * sizeof(char*));
            }
        }
    #endif

    #ifndef BRUTER_TYPELESS
        if (list->types != NULL)
        {
            list->types[0] = 0; // reset type to 0
            if (list->size > 1) 
            {
                memmove(&(list->types[0]), &(list->types[1]), (list->size - 1) * sizeof(int8_t));
            }
        }
    #endif

    list->size--;
    return ret;
}

static inline BruterValue _bruter_remove(BruterList *list, BruterInt i)
{
    BruterValue ret = list->data[i];
    memmove(&(list->data[i]), &(list->data[i + 1]), (list->size - (i) - 1) * sizeof(BruterValue));
    list->size--;

    #ifndef BRUTER_KEYLESS
        if (list->keys != NULL)
        {
            free(list->keys[i]);
            list->keys[i] = NULL;
            memmove(&(list->keys[i]), &(list->keys[i + 1]), (list->size - (i) - 1) * sizeof(char*));
        }
    #endif

    #ifndef BRUTER_TYPELESS
        if (list->types != NULL)
        {
            list->types[i] = 0; // reset type to 0
            memmove(&(list->types[i]), &(list->types[i + 1]), (list->size - (i) - 1) * sizeof(int8_t));
        }
    #endif

    return ret;
}

static inline BruterValue _bruter_fast_remove(BruterList *list, BruterInt i)
{
    BruterValue ret = list->data[i];
    _bruter_swap(list, i, list->size - 1);
    _bruter_pop(list);
    return ret;
}

static inline void _bruter_swap(BruterList *list, BruterInt i1, BruterInt i2)
{
    BruterValue tmp = list->data[i1];
    list->data[i1] = list->data[i2];
    list->data[i2] = tmp;

    #ifndef BRUTER_KEYLESS
        if (list->keys != NULL)
        {
            char* tmp_key = list->keys[i1];
            list->keys[i1] = list->keys[i2];
            list->keys[i2] = tmp_key;
        }
    #endif

    #ifndef BRUTER_TYPELESS
        if (list->types != NULL)
        {
            int8_t tmp_type = list->types[i1];
            list->types[i1] = list->types[i2];
            list->types[i2] = tmp_type;
        }
    #endif
}

static inline BruterInt _bruter_find(BruterList *list, BruterValue value, const char* key) 
{
    #ifndef BRUTER_KEYLESS
        if (list->keys != NULL && key != NULL)
        {
            for (BruterInt i = 0; i < list->size; i++)
            {
                if (list->keys[i] && strcmp(list->keys[i], key) == 0)
                {
                    return i;
                }
            }
        }
        else if (list->keys == NULL && key != NULL)
        {
            printf("BRUTER_ERROR: _bruter_find received a key but the list is not a table\n");
            return -1;
        }
        else
        {
    #endif
            // note this loop is not inside any macro
            for (BruterInt i = 0; i < list->size; i++)
            {
                if (list->data[i].i == value.i)
                {
                    return i;
                }
            }

    // close the previous if, only if we are not in a keyless mode
    #ifndef BRUTER_KEYLESS
        }
    #endif

    return -1;
}

static inline void _bruter_reverse(BruterList *list)
{
    for (BruterInt i = 0; i < list->size / 2; i++)
    {
        _bruter_swap(list, i, list->size - i - 1);
    }
}

static inline BruterList* _bruter_copy(BruterList *list)
{
    BruterList *copy = _bruter_init(list->capacity
    #ifndef BRUTER_KEYLESS
        , list->keys != NULL
    #else
        , false
    #endif
    #ifndef BRUTER_TYPELESS
        , list->types != NULL
    #else
        , false
    #endif
    );
    
    if (copy == NULL)
    {
        printf("BRUTER_ERROR: failed to allocate memory for BruterList copy\n");
        exit(EXIT_FAILURE);
    }
    copy->size = list->size;
    memcpy(copy->data, list->data, copy->size * sizeof(BruterValue));
    
    #ifndef BRUTER_KEYLESS
        if (list->keys != NULL)
        {
            for (BruterInt i = 0; i < copy->size; i++)
            {
                if (list->keys[i] != NULL)
                {
                    int len = strlen(list->keys[i]);
                    copy->keys[i] = malloc(len + 1);
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
    #endif

    #ifndef BRUTER_TYPELESS
        if (list->types != NULL)
        {
            copy->types = (int8_t*)malloc(copy->capacity * sizeof(int8_t));
            if (copy->types == NULL)
            {
                printf("BRUTER_ERROR: failed to allocate memory for BruterList types copy\n");
                _bruter_free(copy);
                exit(EXIT_FAILURE);
            }
            memcpy(copy->types, list->types, copy->size * sizeof(int8_t));
        }
        else
        {
            copy->types = NULL;
        }
    #endif

    
    copy->capacity = list->capacity;

    return copy;
}

static inline void _bruter_concat(BruterList *dest, BruterList *src)
{
    if (dest->size + src->size > dest->capacity)
    {
        while (dest->size + src->size > dest->capacity)
        {
            _bruter_double(dest);
        }
    }
    
    memcpy(&(dest->data[dest->size]), src->data, src->size * sizeof(BruterValue));

    #ifndef BRUTER_KEYLESS
        if (dest->keys != NULL && src->keys != NULL)
        {
            for (BruterInt i = 0; i < src->size; i++)
            {
                if (src->keys[i] != NULL)
                {
                    int len = strlen(src->keys[i]);
                    dest->keys[dest->size + i] = malloc(len + 1);
                    strcpy(dest->keys[dest->size + i], src->keys[i]);
                }
                else 
                {
                    dest->keys[dest->size + i] = NULL;
                }
            }
        }
        else if (dest->keys != NULL)
        {
            for (BruterInt i = 0; i < src->size; i++)
            {
                dest->keys[dest->size + i] = NULL; // fill with NULL if keys are not present in src
            }
        }
    #endif

    #ifndef BRUTER_TYPELESS
        if (dest->types != NULL && src->types != NULL)
        {
            memcpy(&(dest->types[dest->size]), src->types, src->size * sizeof(int8_t));
        }
        else if (dest->types != NULL)
        {
            memset(&(dest->types[dest->size]), 0, src->size * sizeof(int8_t)); // fill with 0 if types are not present in src
        }
    #endif

    dest->size += src->size;
}

// pass NULL for context if you want to call a function directly
// if context exist, the return will be always an int, because it return the index of the result in context
static inline BruterValue _bruter_call(BruterList *context, BruterList *list)
{
    BruterValue(*_function)(BruterList*, BruterList*);
    if (context)
    {
        _function = context->data[list->data[0].i].p;
        return (BruterValue){.i = _function(context, list).i};
    }
    else 
    {
        _function = list->data[0].p;
        return _function(NULL, list);
    }
}


static inline BruterValue _bruter_get(BruterList *list, BruterInt i)
{
    if (i < 0 || i >= list->size)
    {
        return _bruter_value_i(-1); // return -1 if index is out of range
    }
    return list->data[i];
}

static inline void _bruter_set(BruterList *list, BruterInt i, BruterValue value)
{
    if (i < 0 || i >= list->size)
    {
        printf("BRUTER_ERROR: index %" PRIdPTR " out of range in list of size %" PRIdPTR " \n", i, list->size);
        exit(EXIT_FAILURE);
    }
    list->data[i] = value;
}

#ifndef BRUTER_KEYLESS
    static inline char* _bruter_get_key(BruterList *list, BruterInt i)
    {
        if (list->keys == NULL)
        {
            printf("BRUTER_ERROR: _bruter_get_key called on a list that is not a table\n");
            exit(EXIT_FAILURE);
        }
        
        if (i < 0 || i >= list->size)
        {
            printf("BRUTER_ERROR: index %" PRIdPTR " out of range in list of size %" PRIdPTR " \n", i, list->size);
            return NULL;
        }
        
        return list->keys[i];
    }

    static inline void _bruter_set_key(BruterList *list, BruterInt i, const char *key)
    {
        if (list->keys == NULL)
        {
            printf("BRUTER_ERROR: _bruter_set_key called on a list that is not a table\n");
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
            int len = strlen(key);
            list->keys[i] = malloc(len + 1);
            strcpy(list->keys[i], key);
        }
        else 
        {
            list->keys[i] = NULL;
        }
    }
#endif

#ifndef BRUTER_TYPELESS
    static inline int8_t _bruter_get_type(BruterList *list, BruterInt i)
    {
        if (list->types == NULL)
        {
            printf("BRUTER_ERROR: _bruter_get_type called on a list that is not typed\n");
            exit(EXIT_FAILURE);
        }
        
        if (i < 0 || i >= list->size)
        {
            printf("BRUTER_ERROR: index %" PRIdPTR " out of range in list of size %" PRIdPTR " \n", i, list->size);
            return 0; // return 0 if index is out of range
        }
        
        return list->types[i];
    }

    static inline void _bruter_set_type(BruterList *list, BruterInt i, int8_t type)
    {
        if (list->types == NULL)
        {
            printf("BRUTER_ERROR: _bruter_set_type called on a list that is not typed\n");
            exit(EXIT_FAILURE);
        }
        
        if (i < 0 || i >= list->size)
        {
            printf("BRUTER_ERROR: index %" PRIdPTR " out of range in list of size %" PRIdPTR " \n", i, list->size);
            exit(EXIT_FAILURE);
        }
        
        list->types[i] = type;
    }

#endif

#endif