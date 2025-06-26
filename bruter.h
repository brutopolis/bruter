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
#define VERSION "0.9.0"

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

static inline BruterValue bruter_value_i(BruterInt value);
static inline BruterValue bruter_value_u(BruterUInt value);
static inline BruterValue bruter_value_f(BruterFloat value);
static inline BruterValue bruter_value_p(void *value);

// BruterList functions(also work for tables as tables are just lists with keys)
// create a new list with the given size, if size is 0, it will be initialized with NULL data and then allocated when needed
static inline BruterList   *bruter_new(BruterInt size, bool is_table, bool is_typed);
// free the list 
static inline void          bruter_free(BruterList *list);
// double the list capacity
static inline void          bruter_double(BruterList *list);
// halve the list capacity
static inline void          bruter_half(BruterList *list);
// push a value to the end of the list
static inline void          bruter_push(BruterList *list, BruterValue value, const char* key, int8_t type);
static inline void          bruter_push_int(BruterList *list, BruterInt value, const char* key);
static inline void          bruter_push_uint(BruterList *list, BruterUInt value, const char* key);
static inline void          bruter_push_float(BruterList *list, BruterFloat value, const char* key);
static inline void          bruter_push_pointer(BruterList *list, void *value, const char* key);
// unshift a value to the start of the list
static inline void          bruter_unshift(BruterList *list, BruterValue value, const char* key, int8_t type);
static inline void          bruter_unshift_int(BruterList *list, BruterInt value, const char* key);
static inline void          bruter_unshift_uint(BruterList *list, BruterUInt value, const char* key);
static inline void          bruter_unshift_float(BruterList *list, BruterFloat value, const char* key);
static inline void          bruter_unshift_pointer(BruterList *list, void *value, const char* key);
// insert a value at index i in the list
static inline void          bruter_insert(BruterList *list, BruterInt i, BruterValue value, const char* key, int8_t type);
static inline void          bruter_insert_int(BruterList *list, BruterInt i, BruterInt value, const char* key);
static inline void          bruter_insert_uint(BruterList *list, BruterInt i, BruterUInt value, const char* key);
static inline void          bruter_insert_float(BruterList *list, BruterInt i, BruterFloat value, const char* key);
static inline void          bruter_insert_pointer(BruterList *list, BruterInt i, void *value, const char* key);
// pop a value from the end of the list
static inline BruterValue   bruter_pop(BruterList *list);
// shift a value from the start of the list
static inline BruterValue   bruter_shift(BruterList *list);
// remove and return the value at index i in the list, shifting the rest of the list
static inline BruterValue   bruter_remove(BruterList *list, BruterInt i);
// fast remove a value at index i in the list, swapping it with the last element and popping it
static inline BruterValue   bruter_fast_remove(BruterList *list, BruterInt i);
// swap two values in the list at indices i1 and i2
static inline void          bruter_swap(BruterList *list, BruterInt i1, BruterInt i2);
// find the index of a value in the list, returns -1 if not found
static inline BruterInt     bruter_find(BruterList *list, BruterValue value);
static inline BruterInt     bruter_find_int(BruterList *list, BruterInt value);
static inline BruterInt     bruter_find_uint(BruterList *list, BruterUInt value);
static inline BruterInt     bruter_find_float(BruterList *list, BruterFloat value);
static inline BruterInt     bruter_find_pointer(BruterList *list, void *value);
static inline BruterInt     bruter_find_key(BruterList *list, const char* key);
// reverse the list in place
static inline void          bruter_reverse(BruterList *list);
// create a copy of the list, with the same capacity and size, but new data array
static inline BruterList   *bruter_copy(BruterList *list);
// concatenate two lists, appending src to dest, resizing dest if necessary
static inline void          bruter_concat(BruterList *dest, BruterList *src);
// receive a context and a list of indexes relative to the context, and call it as a stack
// the function pointer must have the same type as bruter_call BruterInt(*)(BruterList*, BruterList*);
static inline BruterInt     bruter_call(BruterList *context, BruterList *list);
// same as bruter_call, but it will run the list as a stack without any relative context
// the function pointer must have the same type as bruter_run BruterValue(*)(BruterList*);
static inline BruterValue   bruter_run(BruterList *list);
// get a value at index i in the list, returns a value with i set to -1 if index is out of range
static inline BruterValue   bruter_get(BruterList *list, BruterInt i);
// set a value at index i in the list, if index is out of range, it will print an error and exit
static inline void          bruter_set(BruterList *list, BruterInt i, BruterValue value);
static inline void          bruter_set_int(BruterList *list, BruterInt i, BruterInt value);
static inline void          bruter_set_uint(BruterList *list, BruterInt i, BruterUInt value);
static inline void          bruter_set_float(BruterList *list, BruterInt i, BruterFloat value);
static inline void          bruter_set_pointer(BruterList *list, BruterInt i, void *value);
static inline void          bruter_set_string(BruterList *list, BruterInt i, const char *value);
// get a key at index i in the list, returns NULL if index is out of range or if the list is not a table
static inline char*         bruter_get_key(BruterList *list, BruterInt i);
// set a key at index i in the list, if index is out of range, it will print an error and exit
static inline void          bruter_set_key(BruterList *list, BruterInt i , const char *key);
// get a type at index i in the list, returns 0 if index is out of range or if the list is not typed
static inline int8_t        bruter_get_type(BruterList *list, BruterInt i);
// set a type at index i in the list, if index is out of range, it will print an error and exit
static inline void          bruter_set_type(BruterList *list, BruterInt i, int8_t type);

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

static inline BruterList *bruter_new(BruterInt size, bool is_table, bool is_typed)
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

    return list;
}

static inline void bruter_free(BruterList *list)
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

    if (list->size > list->capacity)
    {
        list->size = list->capacity;
    }
}

static inline void bruter_push(BruterList *list, BruterValue value, const char* key, int8_t type)
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

static inline void bruter_push_int(BruterList *list, BruterInt value, const char* key)
{
    bruter_push(list, bruter_value_i(value), key, 0);
}

static inline void bruter_push_uint(BruterList *list, BruterUInt value, const char* key)
{
    bruter_push(list, bruter_value_u(value), key, 0);
}

static inline void bruter_push_float(BruterList *list, BruterFloat value, const char* key)
{
    bruter_push(list, bruter_value_f(value), key, 1);
}

static inline void bruter_push_pointer(BruterList *list, void *value, const char* key)
{
    bruter_push(list, bruter_value_p(value), key, 2);
}

static inline void bruter_unshift(BruterList *list, BruterValue value, const char* key, int8_t type)
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

static inline void bruter_unshift_int(BruterList *list, BruterInt value, const char* key)
{
    bruter_unshift(list, bruter_value_i(value), key, 0);
}

static inline void bruter_unshift_uint(BruterList *list, BruterUInt value, const char* key)
{
    bruter_unshift(list, bruter_value_u(value), key, 0);
}

static inline void bruter_unshift_float(BruterList *list, BruterFloat value, const char* key)
{
    bruter_unshift(list, bruter_value_f(value), key, 1);
}

static inline void bruter_unshift_pointer(BruterList *list, void *value, const char* key)
{
    bruter_unshift(list, bruter_value_p(value), key, 2);
}

static inline void bruter_insert(BruterList *list, BruterInt i, BruterValue value, const char* key, int8_t type)
{
    if (list->size == list->capacity)
    {
        bruter_double(list);
    }

    if (i <= list->size) 
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

static inline void bruter_insert_int(BruterList *list, BruterInt i, BruterInt value, const char* key)
{
    bruter_insert(list, i, bruter_value_i(value), key, 0);
}

static inline void bruter_insert_uint(BruterList *list, BruterInt i, BruterUInt value, const char* key)
{
    bruter_insert(list, i, bruter_value_u(value), key, 0);
}

static inline void bruter_insert_float(BruterList *list, BruterInt i, BruterFloat value, const char* key)
{
    bruter_insert(list, i, bruter_value_f(value), key, 1);
}

static inline void bruter_insert_pointer(BruterList *list, BruterInt i, void *value, const char* key)
{
    bruter_insert(list, i, bruter_value_p(value), key, 2);
}

static inline BruterValue bruter_pop(BruterList *list)
{
    if (list->size == 0)
    {
        printf("BRUTER_ERROR: cannot pop from empty list\n");
        exit(EXIT_FAILURE);
    }

    if (list->keys != NULL)
    {
        free(list->keys[list->size - 1]);
        list->keys[list->size - 1] = NULL;
    }

    if (list->types != NULL)
    {
        list->types[list->size - 1] = 0; // reset type to 0
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

    if (list->keys != NULL)
    {
        free(list->keys[0]);
        list->keys[0] = NULL;
        if (list->size > 1) 
        {
            memmove(&(list->keys[0]), &(list->keys[1]), (list->size - 1) * sizeof(char*));
        }
    }

    if (list->types != NULL)
    {
        list->types[0] = 0; // reset type to 0
        if (list->size > 1) 
        {
            memmove(&(list->types[0]), &(list->types[1]), (list->size - 1) * sizeof(int8_t));
        }
    }

    list->size--;
    return ret;
}

static inline BruterValue bruter_remove(BruterList *list, BruterInt i)
{
    BruterValue ret = list->data[i];
    memmove(&(list->data[i]), &(list->data[i + 1]), (list->size - (i) - 1) * sizeof(BruterValue));
    list->size--;

    if (list->keys != NULL)
    {
        free(list->keys[i]);
        list->keys[i] = NULL;
        memmove(&(list->keys[i]), &(list->keys[i + 1]), (list->size - (i) - 1) * sizeof(char*));
    }

    if (list->types != NULL)
    {
        list->types[i] = 0; // reset type to 0
        memmove(&(list->types[i]), &(list->types[i + 1]), (list->size - (i) - 1) * sizeof(int8_t));
    }

    return ret;
}

static inline BruterValue bruter_fast_remove(BruterList *list, BruterInt i)
{
    bruter_swap(list, i, list->size - 1);
    return bruter_pop(list);
}

static inline void bruter_swap(BruterList *list, BruterInt i1, BruterInt i2)
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

static inline BruterInt bruter_find_int(BruterList *list, BruterInt value)
{
    return bruter_find(list, bruter_value_i(value));
}

static inline BruterInt bruter_find_uint(BruterList *list, BruterUInt value)
{
    return bruter_find(list, bruter_value_u(value));
}

static inline BruterInt bruter_find_float(BruterList *list, BruterFloat value)
{
    return bruter_find(list, bruter_value_f(value));
}

static inline BruterInt bruter_find_pointer(BruterList *list, void *value)
{
    return bruter_find(list, bruter_value_p(value));
}

static inline BruterInt bruter_find_key(BruterList *list, const char* key)
{
    if (list->keys == NULL)
    {
        printf("BRUTER_ERROR: bruter_find_key called on a list that is not a table\n");
        exit(EXIT_FAILURE);
    }

    for (BruterInt i = 0; i < list->size; i++)
    {
        if (list->keys[i] != NULL && strcmp(list->keys[i], key) == 0)
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
    BruterList *copy = bruter_new(list->capacity, list->keys != NULL, list->types != NULL);
    
    if (copy == NULL)
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
    
    if (list->types != NULL)
    {
        copy->types = (int8_t*)malloc(copy->capacity * sizeof(int8_t));
        if (copy->types == NULL)
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
static inline BruterInt bruter_call(BruterList *context, BruterList *list)
{
    if (list->size == 0)
    {
        printf("BRUTER_ERROR: cannot call an empty list\n");
        exit(EXIT_FAILURE);
    }

    BruterInt(*_function)(BruterList*, BruterList*);
    _function = context->data[list->data[0].i].p;
    return _function(context, list);
}

static inline BruterValue bruter_run(BruterList *list)
{
    if (list->size == 0)
    {
        printf("BRUTER_ERROR: cannot run an empty list\n");
        exit(EXIT_FAILURE);
    }

    BruterValue(*_function)(BruterList*);
    _function = list->data[0].p;
    return _function(list);
}

static inline BruterValue bruter_get(BruterList *list, BruterInt i)
{
    if (i < 0 || i >= list->size)
    {
        return bruter_value_i(-1); // return -1 if index is out of range
    }
    return list->data[i];
}

static inline void bruter_set(BruterList *list, BruterInt i, BruterValue value)
{
    if (i < 0 || i >= list->size)
    {
        printf("BRUTER_ERROR: index %" PRIdPTR " out of range in list of size %" PRIdPTR " \n", i, list->size);
        exit(EXIT_FAILURE);
    }
    list->data[i] = value;
}

static inline void bruter_set_int(BruterList *list, BruterInt i, BruterInt value)
{
    bruter_set(list, i, bruter_value_i(value));
}

static inline void bruter_set_uint(BruterList *list, BruterInt i, BruterUInt value)
{
    bruter_set(list, i, bruter_value_u(value));
}

static inline void bruter_set_float(BruterList *list, BruterInt i, BruterFloat value)
{
    bruter_set(list, i, bruter_value_f(value));
}

static inline void bruter_set_pointer(BruterList *list, BruterInt i, void *value)
{
    bruter_set(list, i, bruter_value_p(value));
}

static inline char* bruter_get_key(BruterList *list, BruterInt i)
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

static inline void bruter_set_key(BruterList *list, BruterInt i, const char *key)
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
        int len = strlen(key);
        list->keys[i] = malloc(len + 1);
        strcpy(list->keys[i], key);
    }
    else 
    {
        list->keys[i] = NULL;
    }
}

static inline int8_t bruter_get_type(BruterList *list, BruterInt i)
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

static inline void bruter_set_type(BruterList *list, BruterInt i, int8_t type)
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

// generics were only introduced in C11, so we need to check if the compiler supports it, because bruter is meant to be C99 compatible
// if you have generics it will probably detect it, but if you have the _Generic but it hasnt been autodetected, you can define BRUTER_USE_GENERIC manually
#ifndef BRUTER_USE_GENERIC
    #if defined(__TINYC__) || defined(__TCC__)
        // ok to warn because we know TCC have warning macro
        #warning "Using C11 generic selection feature"
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
        #warning "Using C11 generic selection feature"
        #define BRUTER_USE_GENERIC 1
    #elif defined(__clang__)
        #warning "Using C11 generic selection feature"
        #define BRUTER_USE_GENERIC 1
    #elif defined(_MSC_VER) && _MSC_VER >= 1900  // Visual Studio 2015+
        #warning "C11 generic selection feature not available in MSVC"
        #define BRUTER_USE_GENERIC 0
    #else
        #warning "C11 generic selection feature not available"
        #define BRUTER_USE_GENERIC 0
    #endif
#endif

#if BRUTER_USE_GENERIC
    #define bvalue(a) \
    _Generic((a), \
        uint8_t: bruter_value_u, \
        uint16_t: bruter_value_u, \
        uint32_t: bruter_value_u, \
        uint64_t: bruter_value_u, \
        float: bruter_value_f, \
        double: bruter_value_f, \
        void*: bruter_value_p, \
        char*: bruter_value_p, \
        default: bruter_value_i \
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