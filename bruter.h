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
#define BRUTER_VERSION "0.9.0"

typedef intptr_t BruterInt;
typedef uintptr_t BruterUInt;

// BRUTER use BruterInt and BruterFloat instead of int and float because we need to use always the pointer size for any type that might share the fundamental union type;
// BRUTER use a union as universal type, and BRUTER is able to manipulate and use pointers direcly so we need to use the pointer size;
#if INTPTR_MAX == INT64_MAX
typedef double BruterFloat;
#else
typedef float BruterFloat;
#endif

// some optimization macros
// gcc specific macros for branch prediction and prefetching
// not really necessary, but it can help in some cases
#if defined(BRUTER_AS_LIB)
    #define STATIC_INLINE // we dont want it static inlined
#else
    #define STATIC_INLINE static inline
#endif

// 
#if defined(BRUTER_NO_EXPECT) || defined(BRUTER_NO_PREFETCH)
    #define likely(x)   (x)
    #define unlikely(x) (x)
    #define PREFETCH(addr, rw, locality) ((void)0)
#elif defined(__GNUC__) || defined(__clang__) || defined(BRUTER_FORCE_EXPECT)
    #define likely(x)   __builtin_expect(!!(x), 1)
    #define unlikely(x) __builtin_expect(!!(x), 0)
    #define PREFETCH(addr, rw, locality) __builtin_prefetch(addr, rw, locality)
#else
    #define likely(x)   (x)
    #define unlikely(x) (x)
    #define PREFETCH(addr, rw, locality) ((void)0)
#endif

// BruterValue
// if you are under a pure C99 eviroment, beware of the use of the union,
// C99 defines that the used type is always the last it was defined to,
// you can use memcpy to "pun" the type easily, although most modern compilers doesnt care about this specific union behavior
typedef union 
{
    // all types depend on the size of the pointer
    BruterInt i;
    BruterFloat f;
    BruterUInt u;
    void* p;
} BruterValue;

typedef struct 
{
    BruterInt capacity;
    BruterInt size;
    char **keys;
    int8_t *types;
    BruterValue *data;
} BruterList;

STATIC_INLINE BruterValue bruter_value_int(BruterInt value);
STATIC_INLINE BruterValue bruter_value_uint(BruterUInt value);
STATIC_INLINE BruterValue bruter_value_float(BruterFloat value);
STATIC_INLINE BruterValue bruter_value_pointer(void *value);

// BruterList functions(also work for tables as tables are just lists with keys)
// create a new list with the given size, if size is 0, it will be initialized with NULL data and then allocated when needed
STATIC_INLINE BruterList*   bruter_new(BruterInt size, bool is_table, bool is_typed);
// free the list 
STATIC_INLINE void          bruter_free(BruterList *list);
// double the list capacity
STATIC_INLINE void          bruter_double(BruterList *list);
// halve the list capacity
STATIC_INLINE void          bruter_half(BruterList *list);
// push a value to the end of the list
STATIC_INLINE void          bruter_push(BruterList *list, BruterValue value, const char* key, int8_t type);
STATIC_INLINE void          bruter_push_int(BruterList *list, BruterInt value, const char* key);
STATIC_INLINE void          bruter_push_uint(BruterList *list, BruterUInt value, const char* key);
STATIC_INLINE void          bruter_push_float(BruterList *list, BruterFloat value, const char* key);
STATIC_INLINE void          bruter_push_pointer(BruterList *list, void *value, const char* key);
// unshift a value to the start of the list
STATIC_INLINE void          bruter_unshift(BruterList *list, BruterValue value, const char* key, int8_t type);
STATIC_INLINE void          bruter_unshift_int(BruterList *list, BruterInt value, const char* key);
STATIC_INLINE void          bruter_unshift_uint(BruterList *list, BruterUInt value, const char* key);
STATIC_INLINE void          bruter_unshift_float(BruterList *list, BruterFloat value, const char* key);
STATIC_INLINE void          bruter_unshift_pointer(BruterList *list, void *value, const char* key);
// insert a value at index i in the list
STATIC_INLINE void          bruter_insert(BruterList *list, BruterInt i, BruterValue value, const char* key, int8_t type);
STATIC_INLINE void          bruter_insert_int(BruterList *list, BruterInt i, BruterInt value, const char* key);
STATIC_INLINE void          bruter_insert_uint(BruterList *list, BruterInt i, BruterUInt value, const char* key);
STATIC_INLINE void          bruter_insert_float(BruterList *list, BruterInt i, BruterFloat value, const char* key);
STATIC_INLINE void          bruter_insert_pointer(BruterList *list, BruterInt i, void *value, const char* key);
// pop a value from the end of the list
STATIC_INLINE BruterValue   bruter_pop(BruterList *list);
STATIC_INLINE BruterInt     bruter_pop_int(BruterList *list);
STATIC_INLINE BruterUInt    bruter_pop_uint(BruterList *list);
STATIC_INLINE BruterFloat   bruter_pop_float(BruterList *list);
STATIC_INLINE void*         bruter_pop_pointer(BruterList *list);
// shift a value from the start of the list
STATIC_INLINE BruterValue   bruter_shift(BruterList *list);
STATIC_INLINE BruterInt     bruter_shift_int(BruterList *list);
STATIC_INLINE BruterUInt    bruter_shift_uint(BruterList *list);
STATIC_INLINE BruterFloat   bruter_shift_float(BruterList *list);
STATIC_INLINE void*         bruter_shift_pointer(BruterList *list);
// remove and return the value at index i in the list, shifting the rest of the list
STATIC_INLINE BruterValue   bruter_remove(BruterList *list, BruterInt i);
STATIC_INLINE BruterInt     bruter_remove_int(BruterList *list, BruterInt i);
STATIC_INLINE BruterUInt    bruter_remove_uint(BruterList *list, BruterInt i);
STATIC_INLINE BruterFloat   bruter_remove_float(BruterList *list, BruterInt i);
STATIC_INLINE void*         bruter_remove_pointer(BruterList *list, BruterInt i);

// fast remove a value at index i in the list, swapping it with the last element and popping it
STATIC_INLINE BruterValue   bruter_fast_remove(BruterList *list, BruterInt i);
STATIC_INLINE BruterInt     bruter_fast_remove_int(BruterList *list, BruterInt i);
STATIC_INLINE BruterUInt    bruter_fast_remove_uint(BruterList *list, BruterInt i);
STATIC_INLINE BruterFloat   bruter_fast_remove_float(BruterList *list, BruterInt i);
STATIC_INLINE void*         bruter_fast_remove_pointer(BruterList *list, BruterInt i);
// swap two values in the list at indices i1 and i2
STATIC_INLINE void          bruter_swap(BruterList *list, BruterInt i1, BruterInt i2);
// find the index of a value in the list, returns -1 if not found
STATIC_INLINE BruterInt     bruter_find(BruterList *list, BruterValue value);
STATIC_INLINE BruterInt     bruter_find_int(BruterList *list, BruterInt value);
STATIC_INLINE BruterInt     bruter_find_uint(BruterList *list, BruterUInt value);
STATIC_INLINE BruterInt     bruter_find_float(BruterList *list, BruterFloat value);
STATIC_INLINE BruterInt     bruter_find_pointer(BruterList *list, void *value);
STATIC_INLINE BruterInt     bruter_find_key(BruterList *list, const char* key);
// reverse the list in place
STATIC_INLINE void          bruter_reverse(BruterList *list);
// create a copy of the list, with the same capacity and size, but new data array
STATIC_INLINE BruterList*   bruter_copy(BruterList *list);
// concatenate two lists, appending src to dest, resizing dest if necessary
STATIC_INLINE void          bruter_concat(BruterList *dest, BruterList *src);
// receive a context and a list of indexes relative to the context, and call it as a stack
// the function pointer must have the same type as bruter_call BruterInt(*)(BruterList*, BruterList*);
STATIC_INLINE BruterInt     bruter_call(BruterList *context, BruterList *list);
// same as bruter_call, but it will run the list as a stack without any relative context
// the function pointer must have the same type as bruter_run BruterValue(*)(BruterList*);
STATIC_INLINE BruterValue   bruter_run(BruterList *list);
STATIC_INLINE BruterInt     bruter_run_int(BruterList *list);
STATIC_INLINE BruterUInt    bruter_run_uint(BruterList *list);
STATIC_INLINE BruterFloat   bruter_run_float(BruterList *list);
STATIC_INLINE void*         bruter_run_pointer(BruterList *list);
// get a value at index i in the list, returns a value with i set to -1 if index is out of range
STATIC_INLINE BruterValue   bruter_get(BruterList *list, BruterInt i);
STATIC_INLINE BruterInt     bruter_get_int(BruterList *list, BruterInt i);
STATIC_INLINE BruterUInt    bruter_get_uint(BruterList *list, BruterInt i);
STATIC_INLINE BruterFloat   bruter_get_float(BruterList *list, BruterInt i);
STATIC_INLINE void*         bruter_get_pointer(BruterList *list, BruterInt i);
// set a value at index i in the list, if index is out of range, it will print an error and exit
STATIC_INLINE void          bruter_set(BruterList *list, BruterInt i, BruterValue value);
STATIC_INLINE void          bruter_set_int(BruterList *list, BruterInt i, BruterInt value);
STATIC_INLINE void          bruter_set_uint(BruterList *list, BruterInt i, BruterUInt value);
STATIC_INLINE void          bruter_set_float(BruterList *list, BruterInt i, BruterFloat value);
STATIC_INLINE void          bruter_set_pointer(BruterList *list, BruterInt i, void *value);
// get a key at index i in the list, returns NULL if index is out of range or if the list is not a table
STATIC_INLINE char*         bruter_get_key(BruterList *list, BruterInt i);
// set a key at index i in the list, if index is out of range, it will print an error and exit
STATIC_INLINE void          bruter_set_key(BruterList *list, BruterInt i , const char *key);
// get a type at index i in the list, returns 0 if index is out of range or if the list is not typed
STATIC_INLINE int8_t        bruter_get_type(BruterList *list, BruterInt i);
// set a type at index i in the list, if index is out of range, it will print an error and exit
STATIC_INLINE void          bruter_set_type(BruterList *list, BruterInt i, int8_t type);

// functions implementations
// functions implementations
// functions implementations
// functions implementations

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
    
    if (unlikely(list == NULL))
    {
        printf("BRUTER_ERROR: failed to allocate memory for BruterList\n");
        exit(EXIT_FAILURE);
    }
    
    list->data = (unlikely(size == 0)) ? NULL : (BruterValue*)malloc(size * sizeof(BruterValue));
    
    if (unlikely(size > 0) && unlikely(list->data == NULL))
    {
        printf("BRUTER_ERROR: failed to allocate memory for BruterList data\n");
        free(list);
        exit(EXIT_FAILURE);
    }
    
    list->size = 0;
    list->capacity = size;

    if (is_table)
    {
        list->keys = (char**)calloc(size, sizeof(char*)); // we need all keys to be NULL
        if (unlikely(list->keys == NULL))
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

    if (is_typed)
    {
        list->types = (int8_t*)calloc(size, sizeof(int8_t)); // we need all types to be 0
        if (unlikely(list->types == NULL))
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
    list->capacity = list->capacity == 0 ? 1 : list->capacity * 2;
    BruterValue *new_data = realloc(list->data, list->capacity * sizeof(BruterValue));
    if (unlikely(!new_data))
    {
        printf("BRUTER_ERROR: failed to reallocate list data\n");
        exit(EXIT_FAILURE);
    }
    list->data = new_data;

    if (list->keys != NULL)
    {
        char **new_keys = realloc(list->keys, list->capacity * sizeof(char*));
        if (unlikely(!new_keys))
        {
            printf("BRUTER_ERROR: failed to reallocate list keys\n");
            exit(EXIT_FAILURE);
        }
        list->keys = new_keys;
        memset(list->keys + list->size, 0, (list->capacity - list->size) * sizeof(char*));
    }
    
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
}

STATIC_INLINE void bruter_half(BruterList *list)
{
    list->capacity /= 2;

    BruterValue *new_data = realloc(list->data, list->capacity * sizeof(BruterValue));
    if (unlikely(!new_data))
    {
        printf("BRUTER_ERROR: failed to reallocate list data\n");
        exit(EXIT_FAILURE);
    }
    list->data = new_data;

    if (list->keys != NULL)
    {
        char **new_keys = realloc(list->keys, list->capacity * sizeof(char*));
        if (unlikely(!new_keys))
        {
            printf("BRUTER_ERROR: failed to reallocate list keys\n");
            exit(EXIT_FAILURE);
        }
        list->keys = new_keys;
    }

    if (list->types != NULL)
    {
        int8_t *new_types = realloc(list->types, list->capacity * sizeof(int8_t));
        if (unlikely(!new_types))
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
            int len = strlen(key);
            list->keys[list->size] = malloc(len + 1);
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

STATIC_INLINE void bruter_push_int(BruterList *list, BruterInt value, const char* key)
{
    bruter_push(list, bruter_value_int(value), key, 0);
}

STATIC_INLINE void bruter_push_uint(BruterList *list, BruterUInt value, const char* key)
{
    bruter_push(list, bruter_value_uint(value), key, 0);
}

STATIC_INLINE void bruter_push_float(BruterList *list, BruterFloat value, const char* key)
{
    bruter_push(list, bruter_value_float(value), key, 1);
}

STATIC_INLINE void bruter_push_pointer(BruterList *list, void *value, const char* key)
{
    bruter_push(list, bruter_value_pointer(value), key, 2);
}

STATIC_INLINE void bruter_unshift(BruterList *list, BruterValue value, const char* key, int8_t type)
{
    if (list->size == list->capacity)
    {
        bruter_double(list);
    }
    memmove(&(list->data[1]), &(list->data[0]), list->size * sizeof(BruterValue));
    list->data[0] = value;
    
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

    if (list->types != NULL)
    {
        memmove(&(list->types[1]), &(list->types[0]), list->size * sizeof(int8_t));
        list->types[0] = 0; // default type is 0
    }

    list->size++;
}

STATIC_INLINE void bruter_unshift_int(BruterList *list, BruterInt value, const char* key)
{
    bruter_unshift(list, bruter_value_int(value), key, 0);
}

STATIC_INLINE void bruter_unshift_uint(BruterList *list, BruterUInt value, const char* key)
{
    bruter_unshift(list, bruter_value_uint(value), key, 0);
}

STATIC_INLINE void bruter_unshift_float(BruterList *list, BruterFloat value, const char* key)
{
    bruter_unshift(list, bruter_value_float(value), key, 1);
}

STATIC_INLINE void bruter_unshift_pointer(BruterList *list, void *value, const char* key)
{
    bruter_unshift(list, bruter_value_pointer(value), key, 2);
}

STATIC_INLINE void bruter_insert(BruterList *list, BruterInt i, BruterValue value, const char* key, int8_t type)
{
    if (unlikely(list->size == list->capacity))
    {
        bruter_double(list);
    }

    if (likely(i <= list->size))
    {
        memmove(&(list->data[i + 1]), &(list->data[i]), (list->size - i) * sizeof(BruterValue));
        list->data[i] = value;
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

        if (list->types != NULL)
        {
            memmove(&(list->types[i + 1]), &(list->types[i]), (list->size - i) * sizeof(int8_t));
            list->types[i] = 0; // default type is 0
        }

        list->size++;
    } 
    else 
    {
        printf("BRUTER_ERROR: index %" PRIdPTR " out of range in list of size %" PRIdPTR " \n", i, list->size);
        exit(EXIT_FAILURE);
    }
}

STATIC_INLINE void bruter_insert_int(BruterList *list, BruterInt i, BruterInt value, const char* key)
{
    bruter_insert(list, i, bruter_value_int(value), key, 0);
}

STATIC_INLINE void bruter_insert_uint(BruterList *list, BruterInt i, BruterUInt value, const char* key)
{
    bruter_insert(list, i, bruter_value_uint(value), key, 0);
}

STATIC_INLINE void bruter_insert_float(BruterList *list, BruterInt i, BruterFloat value, const char* key)
{
    bruter_insert(list, i, bruter_value_float(value), key, 1);
}

STATIC_INLINE void bruter_insert_pointer(BruterList *list, BruterInt i, void *value, const char* key)
{
    bruter_insert(list, i, bruter_value_pointer(value), key, 2);
}

#define BRUTER_FUNCTION_CONTENT_POP() \
    BruterValue ret = list->data[list->size - 1]; \
    do { \
        if (unlikely(list->size == 0))\
        {\
            printf("BRUTER_ERROR: cannot pop from empty list\n");\
            exit(EXIT_FAILURE);\
        }\
\
        if (list->keys != NULL)\
        {\
            free(list->keys[list->size - 1]);\
            list->keys[list->size - 1] = NULL;\
        }\
\
        if (list->types != NULL)\
        {\
            list->types[list->size - 1] = 0; \
        }\
        list->size--; \
    } while(0);
        
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

#define BRUTER_FUNCTION_CONTENT_SHIFT() \
    BruterValue ret = list->data[0]; \
    do { \
        if (unlikely(list->size == 0))\
        {\
            printf("BRUTER_ERROR: cannot shift from empty list\n");\
            exit(EXIT_FAILURE);\
        }\
        if (likely(list->size > 1)) \
        { \
            memmove(&(list->data[0]), &(list->data[1]), (list->size - 1) * sizeof(BruterValue)); \
        } \
        if (list->keys != NULL) \
        { \
            free(list->keys[0]); \
            list->keys[0] = NULL; \
            if (likely(list->size > 1)) \
            { \
                memmove(&(list->keys[0]), &(list->keys[1]), (list->size - 1) * sizeof(char*)); \
            } \
        } \
        if (list->types != NULL) \
        { \
            list->types[0] = 0; /* reset type to 0 */ \
            if (likely(list->size > 1)) \
            { \
                memmove(&(list->types[0]), &(list->types[1]), (list->size - 1) * sizeof(int8_t)); \
            } \
        } \
        list->size--; \
    } while(0);
    
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

#define BRUTER_FUNCTION_CONTENT_REMOVE() \
    BruterValue ret = list->data[i]; \
    do { \
        if (unlikely(list->size == 0))\
        {\
            printf("BRUTER_ERROR: cannot pop from empty list\n");\
            exit(EXIT_FAILURE);\
        }\
        size_t elements_to_move = (list->size - (i) - 1); \
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

STATIC_INLINE BruterInt bruter_find(BruterList *list, BruterValue value) 
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

STATIC_INLINE BruterInt bruter_find_int(BruterList *list, BruterInt value)
{
    return bruter_find(list, bruter_value_int(value));
}

STATIC_INLINE BruterInt bruter_find_uint(BruterList *list, BruterUInt value)
{
    return bruter_find(list, bruter_value_uint(value));
}

STATIC_INLINE BruterInt bruter_find_float(BruterList *list, BruterFloat value)
{
    return bruter_find(list, bruter_value_float(value));
}

STATIC_INLINE BruterInt bruter_find_pointer(BruterList *list, void *value)
{
    return bruter_find(list, bruter_value_pointer(value));
}

STATIC_INLINE BruterInt bruter_find_key(BruterList *list, const char* key)
{
    if (unlikely(list->keys == NULL))
    {
        printf("BRUTER_ERROR: bruter_find_key called on a list that is not a table\n");
        exit(EXIT_FAILURE);
    }

    for (BruterInt i = 0; i < list->size; i++)
    {
        // if it has keys we suppose most of the time that they are not NULL
        if (likely(list->keys[i] != NULL))
        {
            if (unlikely(strcmp(list->keys[i], key) == 0))
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

STATIC_INLINE BruterList* bruter_copy(BruterList *list)
{
    BruterList *copy = bruter_new(list->capacity, list->keys != NULL, list->types != NULL);
    
    if (unlikely(copy == NULL))
    {
        printf("BRUTER_ERROR: failed to allocate memory for BruterList copy\n");
        exit(EXIT_FAILURE);
    }
    copy->size = list->size;
    memcpy(copy->data, list->data, copy->size * sizeof(BruterValue));
    
    if (list->keys != NULL)
    {
        for (BruterInt i = 0; i < copy->size; i++)
        {
            if (likely(list->keys[i] != NULL))
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
    
    if (list->types != NULL)
    {
        copy->types = (int8_t*)malloc(copy->capacity * sizeof(int8_t));
        if (unlikely(copy->types == NULL))
        {
            printf("BRUTER_ERROR: failed to allocate memory for BruterList types copy\n");
            bruter_free(copy);
            exit(EXIT_FAILURE);
        }
        memcpy(copy->types, list->types, copy->size * sizeof(int8_t));
    }
    else
    {
        copy->types = NULL;
    }

    
    copy->capacity = list->capacity;

    return copy;
}

STATIC_INLINE void bruter_concat(BruterList *dest, BruterList *src)
{
    if (dest->size + src->size > dest->capacity)
    {
        while (dest->size + src->size > dest->capacity)
        {
            bruter_double(dest);
        }
    }
    
    memcpy(&(dest->data[dest->size]), src->data, src->size * sizeof(BruterValue));

    if (dest->keys != NULL && src->keys != NULL)
    {
        for (BruterInt i = 0; i < src->size; i++)
        {
            if (likely(src->keys[i] != NULL))
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
    if (dest->types != NULL && src->types != NULL)
    {
        memcpy(&(dest->types[dest->size]), src->types, src->size * sizeof(int8_t));
    }
    else if (dest->types != NULL)
    {
        memset(&(dest->types[dest->size]), 0, src->size * sizeof(int8_t)); // fill with 0 if types are not present in src
    }

    dest->size += src->size;
}

// contextual call
STATIC_INLINE BruterInt bruter_call(BruterList *context, BruterList *list)
{
    BruterInt(*_function)(BruterList*, BruterList*);

    if (unlikely(list->size == 0))
    {
        printf("BRUTER_ERROR: cannot call an empty list\n");
        exit(EXIT_FAILURE);
    }

    _function = context->data[list->data[0].i].p;
    return _function(context, list);
}

#define BRUTER_FUNCTION_CONTENT_RUN() \
    BruterValue(*_function)(BruterList*); \
    do { \
        _function = list->data[0].p; \
        if (unlikely(_function == NULL)) \
        { \
            printf("BRUTER_ERROR: function pointer is NULL\n"); \
            exit(EXIT_FAILURE); \
        }\
    } while(0);

STATIC_INLINE BruterValue bruter_run(BruterList *list)
{
    BRUTER_FUNCTION_CONTENT_RUN();
    return _function(list);
}

STATIC_INLINE BruterInt bruter_run_int(BruterList *list)
{
    BRUTER_FUNCTION_CONTENT_RUN();
    return _function(list).i;
}

STATIC_INLINE BruterUInt bruter_run_uint(BruterList *list)
{
    BRUTER_FUNCTION_CONTENT_RUN();
    return _function(list).u;
}

STATIC_INLINE BruterFloat bruter_run_float(BruterList *list)
{
    BRUTER_FUNCTION_CONTENT_RUN();
    return _function(list).f;
}

STATIC_INLINE void* bruter_run_pointer(BruterList *list)
{
    BRUTER_FUNCTION_CONTENT_RUN();
    return _function(list).p;
}

STATIC_INLINE BruterValue bruter_get(BruterList *list, BruterInt i)
{
    if (unlikely(i < 0 || i >= list->size))
    {
        return bruter_value_int(-1); // return -1 if index is out of range
    }
    return list->data[i];
}

STATIC_INLINE BruterInt bruter_get_int(BruterList *list, BruterInt i)
{
    if (unlikely(i < 0 || i >= list->size))
    {
        return -1; // return -1 if index is out of range
    }
    return list->data[i].i;
}

STATIC_INLINE BruterUInt bruter_get_uint(BruterList *list, BruterInt i)
{
    if (unlikely(i < 0 || i >= list->size))
    {
        return 0; // return 0 if index is out of range
    }
    return list->data[i].u;
}

STATIC_INLINE BruterFloat bruter_get_float(BruterList *list, BruterInt i)
{
    if (unlikely(i < 0 || i >= list->size))
    {
        return 0.0f; // return 0.0f if index is out of range
    }
    return list->data[i].f;
}

STATIC_INLINE void* bruter_get_pointer(BruterList *list, BruterInt i)
{
    if (unlikely(i < 0 || i >= list->size))
    {
        return NULL; // return NULL if index is out of range
    }
    return list->data[i].p;
}

STATIC_INLINE void bruter_set(BruterList *list, BruterInt i, BruterValue value)
{
    if (unlikely(i < 0 || i >= list->size))
    {
        printf("BRUTER_ERROR: index %" PRIdPTR " out of range in list of size %" PRIdPTR " \n", i, list->size);
        exit(EXIT_FAILURE);
    }
    list->data[i] = value;
}

STATIC_INLINE void bruter_set_int(BruterList *list, BruterInt i, BruterInt value)
{
    bruter_set(list, i, bruter_value_int(value));
}

STATIC_INLINE void bruter_set_uint(BruterList *list, BruterInt i, BruterUInt value)
{
    bruter_set(list, i, bruter_value_uint(value));
}

STATIC_INLINE void bruter_set_float(BruterList *list, BruterInt i, BruterFloat value)
{
    bruter_set(list, i, bruter_value_float(value));
}

STATIC_INLINE void bruter_set_pointer(BruterList *list, BruterInt i, void *value)
{
    bruter_set(list, i, bruter_value_pointer(value));
}

STATIC_INLINE char* bruter_get_key(BruterList *list, BruterInt i)
{
    if (unlikely(list->keys == NULL))
    {
        printf("BRUTER_ERROR: bruter_get_key called on a list that is not a table\n");
        exit(EXIT_FAILURE);
    }
    
    if (unlikely(i < 0 || i >= list->size))
    {
        printf("BRUTER_ERROR: index %" PRIdPTR " out of range in list of size %" PRIdPTR " \n", i, list->size);
        return NULL;
    }
    
    return list->keys[i];
}

STATIC_INLINE void bruter_set_key(BruterList *list, BruterInt i, const char *key)
{
    if (unlikely(list->keys == NULL))
    {
        printf("BRUTER_ERROR: bruter_set_key called on a list that is not a table\n");
        exit(EXIT_FAILURE);
    }
    
    if (unlikely(i < 0 || i >= list->size))
    {
        printf("BRUTER_ERROR: index %" PRIdPTR " out of range in list of size %" PRIdPTR " \n", i, list->size);
        exit(EXIT_FAILURE);
    }
    
    // we assume that *most* keys of a list with keys are not NULL
    if (likely(list->keys[i] != NULL))
    {
        free(list->keys[i]);
    }
    
    if (likely(key != NULL))
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

STATIC_INLINE int8_t bruter_get_type(BruterList *list, BruterInt i)
{
    if (unlikely(list->types == NULL))
    {
        printf("BRUTER_ERROR: bruter_get_type called on a list that is not typed\n");
        exit(EXIT_FAILURE);
    }
    
    if (unlikely(i < 0 || i >= list->size))
    {
        printf("BRUTER_ERROR: index %" PRIdPTR " out of range in list of size %" PRIdPTR " \n", i, list->size);
        return 0; // return 0 if index is out of range
    }
    
    return list->types[i];
}

STATIC_INLINE void bruter_set_type(BruterList *list, BruterInt i, int8_t type)
{
    if (unlikely(list->types == NULL))
    {
        printf("BRUTER_ERROR: bruter_set_type called on a list that is not typed\n");
        exit(EXIT_FAILURE);
    }
    
    if (unlikely(i < 0 || i >= list->size))
    {
        printf("BRUTER_ERROR: index %" PRIdPTR " out of range in list of size %" PRIdPTR " \n", i, list->size);
        exit(EXIT_FAILURE);
    }
    
    list->types[i] = type;
}

// generics were only introduced in C11, so we need to check if the compiler supports it, because bruter is meant to be C99 compatible
// if you have generics it will probably detect it, but if you have the _Generic but it hasnt been autodetected, you can define BRUTER_USE_GENERIC manually
#ifndef BRUTER_USE_GENERIC
    #if defined(__TINYC__) || defined(__TCC__)
        #define BRUTER_USE_GENERIC 1
    #elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
        #define BRUTER_USE_GENERIC 1
    #elif defined(__has_feature)
        #if __has_feature(c_generic_selections)
            #define BRUTER_USE_GENERIC 1
        #else
            #define BRUTER_USE_GENERIC 0
        #endif
    #elif defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 9))
        #define BRUTER_USE_GENERIC 1
    #elif defined(__clang__)
        #define BRUTER_USE_GENERIC 1
    #elif defined(_MSC_VER) && _MSC_VER >= 1900  
        #define BRUTER_USE_GENERIC 1
    #else
        #define BRUTER_USE_GENERIC 0
    #endif
#endif

#if BRUTER_USE_GENERIC
    #define bvalue(a) \
    _Generic((a), \
        uint8_t: bruter_value_uint, \
        uint16_t: bruter_value_uint, \
        uint32_t: bruter_value_uint, \
        uint64_t: bruter_value_uint, \
        float: bruter_value_float, \
        double: bruter_value_float, \
        void*: bruter_value_pointer, \
        char*: bruter_value_pointer, \
        default: bruter_value_int \
    )(a)

    // most are simply the same function
    #define bnew bruter_new
    #define bfree bruter_free
    #define bdouble bruter_double
    #define bhalf bruter_half
    #define bpush(list, value, key) \
    _Generic((value), \
        uint8_t: bruter_push_uint, \
        uint16_t: bruter_push_uint, \
        uint32_t: bruter_push_uint, \
        uint64_t: bruter_push_uint, \
        float: bruter_push_float, \
        double: bruter_push_float, \
        void*: bruter_push_pointer, \
        char*: bruter_push_pointer, \
        BruterValue: bruter_push, \
        default: bruter_push_int \
    )(list, value, key)
    #define bunshift(list, value, key) \
    _Generic((value), \
        uint8_t: bruter_unshift_uint, \
        uint16_t: bruter_unshift_uint, \
        uint32_t: bruter_unshift_uint, \
        uint64_t: bruter_unshift_uint, \
        float: bruter_unshift_float, \
        double: bruter_unshift_float, \
        void*: bruter_unshift_pointer, \
        char*: bruter_unshift_pointer, \
        default: bruter_unshift_int \
    )(list, value, key)
    #define binsert(list, i, value, key) \
    _Generic((value), \
        uint8_t: bruter_insert_uint, \
        uint16_t: bruter_insert_uint, \
        uint32_t: bruter_insert_uint, \
        uint64_t: bruter_insert_uint, \
        float: bruter_insert_float, \
        double: bruter_insert_float, \
        void*: bruter_insert_pointer, \
        char*: bruter_insert_pointer, \
        default: bruter_insert_int \
    )(list, i, value, key)
    #define bpop bruter_pop
    #define bshift bruter_shift
    #define bremove bruter_remove
    #define bfast_remove bruter_fast_remove
    #define bswap bruter_swap
    #define bfind(list, value) \
    _Generic((value), \
        uint8_t: bruter_find_uint, \
        uint16_t: bruter_find_uint, \
        uint32_t: bruter_find_uint, \
        uint64_t: bruter_find_uint, \
        float: bruter_find_float, \
        double: bruter_find_float, \
        void*: bruter_find_pointer, \
        char*: bruter_find_pointer, \
        default: bruter_find_int \
    )(list, value)
    #define bfind_key bruter_find_key
    #define breverse bruter_reverse
    #define bcopy bruter_copy
    #define bconcat bruter_concat
    #define bcall bruter_call
    #define brun bruter_run
    #define bget bruter_get
    #define bset(list, i, value) \
    _Generic((value), \
        uint8_t: bruter_set_uint, \
        uint16_t: bruter_set_uint, \
        uint32_t: bruter_set_uint, \
        uint64_t: bruter_set_uint, \
        float: bruter_set_float, \
        double: bruter_set_float, \
        void*: bruter_set_pointer, \
        char*: bruter_set_pointer, \
        default: bruter_set_int \
    )(list, i, value)
    #define bget_key bruter_get_key
    #define bset_key bruter_set_key
    #define bget_type bruter_get_type
    #define bset_type bruter_set_type
#endif

#ifdef BRUTER_USE_SHORT_TYPES
#define Int BruterInt
#define UInt BruterUInt
#define Float BruterFloat
#define Value BruterValue
#define List BruterList
#endif

#endif