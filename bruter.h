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
#define BRUTER_VERSION "0.9.1"

typedef intptr_t BruterInt;
typedef uintptr_t BruterUInt;

// BRUTER use BruterInt and BruterFloat instead of int and float because we need to use always the pointer size for any type that might share the fundamental union type;
// BRUTER use a union as universal type, and BRUTER is able to manipulate and use pointers direcly so we need to use the pointer size;
#if INTPTR_MAX == INT64_MAX
    typedef double BruterFloat;
#else
    typedef float BruterFloat;
#endif

// uncomment the next line to disable manual GCC specific optimizations
// #define BRUTER_NO_GCC_OPTIMIZATIONS 1

// you can build bruter as lib, just define BRUTER_AS_SOURCE before including this header, it will act like a source.c file
#if defined(BRUTER_AS_SOURCE) 
    #if (defined(__GNUC__) || defined(__clang__)) && !defined(BRUTER_NO_GCC_OPTIMIZATIONS)
        // we want it to be visible in the shared library, so we can use it in other files
        #define STATIC_INLINE __attribute__((visibility("default"))) __attribute__((used))
    #else 
        // we dont want it static inlined
        #define STATIC_INLINE 
    #endif
#else
    #define STATIC_INLINE static inline
#endif

// sometimes we need to allow aggregate return, so we define a macro to supress the warning where we need it
// this is useful for functions that return a union type, like bruter_get
#if defined(__GNUC__) || defined(__clang__) && !defined(BRUTER_NO_GCC_OPTIMIZATIONS)
    #define BRUTER_ALLOW_AGGREGATE_RETURN() \
        _Pragma("GCC diagnostic push") \
        _Pragma("GCC diagnostic ignored \"-Waggregate-return\"")
    #define BRUTER_FORBID_AGGREGATE_RETURN() \
        _Pragma("GCC diagnostic pop")
#else
    #define BRUTER_ALLOW_AGGREGATE_RETURN()
    #define BRUTER_FORBID_AGGREGATE_RETURN()
#endif

// sometimes we need to allow non-literal format strings, so we define a macro to supress the gcc warning where we need it
// this is useful for functions that take a format string as an argument, like printf or snprintf
#if (defined(__GNUC__) || defined(__clang__)) && !defined(BRUTER_NO_GCC_OPTIMIZATIONS)
    // dont expect this to work on other compilers, those pragmas are gcc/clang specific features;
    #define BRUTER_ALLOW_NON_LITERAL_FORMAT() \
        _Pragma("GCC diagnostic push") \
        _Pragma("GCC diagnostic ignored \"-Wformat-nonliteral\"")
    #define BRUTER_FORBID_NON_LITERAL_FORMAT() \
        _Pragma("GCC diagnostic pop")
#else
    #define BRUTER_ALLOW_NON_LITERAL_FORMAT()
    #define BRUTER_FORBID_NON_LITERAL_FORMAT()
#endif

// branch prediction macros, only defined for using under gcc or clang
// likely and unlikely are the only macros that arent UPPERCASED, all other macros are standardized to UPPERCASE;
#if (defined(__GNUC__) || defined(__clang__)) && !defined(BRUTER_NO_GCC_OPTIMIZATIONS)
    // dont expect it to work on other compilers, __builtin_expect is a gcc/clang specific feature;
    #define likely(x)   __builtin_expect(!!(x), 1)
    #define unlikely(x) __builtin_expect(!!(x), 0)
#else
    #define likely(x)   (x)
    #define unlikely(x) (x)
#endif

// C++ support
// if the BRUTER_CPP macro is already defined we just ignore this macro
#ifndef BRUTER_CPP
    // if BRUTER_CPP is not defined, we check if we are compiling with a C++ compiler
    // if we are compiling with a C++ compiler, we automatically define BRUTER_CPP
    // if the user is on C++ mode 
    #if defined(__cplusplus)
        // note this is the only check in the entire file
        // if BRUTER_CPP is define we assume the user is using C++ and we will use C++ features
        #define BRUTER_CPP 1
    #else 
        // if we are not compiling with a C++ compiler, we just define BRUTER_CPP to 0
        #define BRUTER_CPP 0
    #endif
#endif

// well, we need to declare the types before, so we can use them in the union
// this is not defined if BRUTER_MANUAL_UNION is defined, so you can define your own types BEFORE including this header
// note you'll need everything below to be defined before including this header too
typedef union  BruterValue BruterValue;
typedef struct BruterList BruterList;
typedef BruterInt (*BruterFunction)(BruterList *context, BruterList *args);

// define BRUTER_MANUAL_UNION to 1 if you want to manually define the BruterValue union
// note that you still need the primitive types in the union: i, f, u, p, fn
// you are strogly disencouraged to use manual union definition, but in some rare cases it is really needed;
// specially when you are in a extremely restricted environment, like a C99 only environment;
/* example of manual union definition:
    #define BRUTER_MANUAL_UNION 1
    struct BruterList; // obligatory, we need it in the union
    union BruterValue
    {
        // BruterInt, BruterFloat and BruterUInt arent defined yet, so we need to use the types we want directly
        long i;
        double f;
        unsigned long u;
        void* p;
        BruterInt (*fn)(struct BruterList *context, struct BruterList *args);
    };
    #include "bruter.h" // now we can include the bruter.h header, which will use the union we defined
*/
#ifndef BRUTER_MANUAL_UNION
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
    BruterInt (*fn)(struct BruterList *context, struct BruterList *args);

    // C++ stuff
    #if BRUTER_CPP
        // C++ conversion operators, so you can use BruterValue as a normal value
        // operator overloads for C++ to convert BruterValue to the primitive types
        /* example:
            BruterValue value = bruter_value_int(42);
            BruterInt i = value; // this will call the conversion operator to BruterInt
            BruterFloat f = value; // this will call the conversion operator to BruterFloat
            BruterUInt u = value; // this will call the conversion operator to BruterUInt
            void* p = value; // this will call the conversion operator to void*
            BruterFunction fn = value; // this will call the conversion operator to BruterFunction
        */
        // int conversion operator
        operator BruterInt() const 
        {
            return i;
        }

        // float conversion operator
        operator BruterFloat() const 
        {
            return f;
        }

        // uint conversion operator
        operator BruterUInt() const 
        {
            return u;
        }

        // pointer conversion operator
        operator void*() const 
        {
            return p;
        }

        // function conversion operator
        operator BruterFunction() const 
        {
            return fn;
        }
    #endif
};
#endif

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


STATIC_INLINE BruterValue bruter_value_int(BruterInt value);
STATIC_INLINE BruterValue bruter_value_uint(BruterUInt value);
STATIC_INLINE BruterValue bruter_value_float(BruterFloat value);
STATIC_INLINE BruterValue bruter_value_pointer(void *value);
STATIC_INLINE BruterValue bruter_value_function(BruterInt (*func)(BruterList *context, BruterList *args));

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
STATIC_INLINE void          bruter_push_int(BruterList *list, BruterInt value, const char* key, int8_t type);
STATIC_INLINE void          bruter_push_uint(BruterList *list, BruterUInt value, const char* key, int8_t type);
STATIC_INLINE void          bruter_push_float(BruterList *list, BruterFloat value, const char* key, int8_t type);
STATIC_INLINE void          bruter_push_pointer(BruterList *list, void *value, const char* key, int8_t type);
STATIC_INLINE void          bruter_push_function(BruterList *list, BruterInt (*func)(BruterList *context, BruterList *args), const char* key, int8_t type);
// unshift a value to the start of the list
STATIC_INLINE void          bruter_unshift(BruterList *list, BruterValue value, const char* key, int8_t type);
STATIC_INLINE void          bruter_unshift_int(BruterList *list, BruterInt value, const char* key, int8_t type);
STATIC_INLINE void          bruter_unshift_uint(BruterList *list, BruterUInt value, const char* key, int8_t type);
STATIC_INLINE void          bruter_unshift_float(BruterList *list, BruterFloat value, const char* key, int8_t type);
STATIC_INLINE void          bruter_unshift_pointer(BruterList *list, void *value, const char* key, int8_t type);
STATIC_INLINE void          bruter_unshift_function(BruterList *list, BruterInt (*func)(BruterList *context, BruterList *args), const char* key, int8_t type);
// insert a value at index i in the list
STATIC_INLINE void          bruter_insert(BruterList *list, BruterInt i, BruterValue value, const char* key, int8_t type);
STATIC_INLINE void          bruter_insert_int(BruterList *list, BruterInt i, BruterInt value, const char* key, int8_t type);
STATIC_INLINE void          bruter_insert_uint(BruterList *list, BruterInt i, BruterUInt value, const char* key, int8_t type);
STATIC_INLINE void          bruter_insert_float(BruterList *list, BruterInt i, BruterFloat value, const char* key, int8_t type);
STATIC_INLINE void          bruter_insert_pointer(BruterList *list, BruterInt i, void *value, const char* key, int8_t type);
STATIC_INLINE void          bruter_insert_function(BruterList *list, BruterInt i, BruterInt (*func)(BruterList *context, BruterList *args), const char* key, int8_t type);
// pop a value from the end of the list
STATIC_INLINE BruterValue   bruter_pop(BruterList *list);
STATIC_INLINE BruterInt     bruter_pop_int(BruterList *list);
STATIC_INLINE BruterUInt    bruter_pop_uint(BruterList *list);
STATIC_INLINE BruterFloat   bruter_pop_float(BruterList *list);
STATIC_INLINE void*         bruter_pop_pointer(BruterList *list);
STATIC_INLINE BruterInt   (*bruter_pop_function(BruterList *list))(BruterList *context, BruterList *args);
// shift a value from the start of the list
STATIC_INLINE BruterValue   bruter_shift(BruterList *list);
STATIC_INLINE BruterInt     bruter_shift_int(BruterList *list);
STATIC_INLINE BruterUInt    bruter_shift_uint(BruterList *list);
STATIC_INLINE BruterFloat   bruter_shift_float(BruterList *list);
STATIC_INLINE void*         bruter_shift_pointer(BruterList *list);
STATIC_INLINE BruterInt   (*bruter_shift_function(BruterList *list))(BruterList *context, BruterList *args);
// remove and return the value at index i in the list, shifting the rest of the list
STATIC_INLINE BruterValue   bruter_remove(BruterList *list, BruterInt i);
STATIC_INLINE BruterInt     bruter_remove_int(BruterList *list, BruterInt i);
STATIC_INLINE BruterUInt    bruter_remove_uint(BruterList *list, BruterInt i);
STATIC_INLINE BruterFloat   bruter_remove_float(BruterList *list, BruterInt i);
STATIC_INLINE void*         bruter_remove_pointer(BruterList *list, BruterInt i);
STATIC_INLINE BruterInt   (*bruter_remove_function(BruterList *list, BruterInt i))(BruterList *context, BruterList *args);
// fast remove a value at index i in the list, swapping it with the last element and popping it
STATIC_INLINE BruterValue   bruter_fast_remove(BruterList *list, BruterInt i);
STATIC_INLINE BruterInt     bruter_fast_remove_int(BruterList *list, BruterInt i);
STATIC_INLINE BruterUInt    bruter_fast_remove_uint(BruterList *list, BruterInt i);
STATIC_INLINE BruterFloat   bruter_fast_remove_float(BruterList *list, BruterInt i);
STATIC_INLINE void*         bruter_fast_remove_pointer(BruterList *list, BruterInt i);
STATIC_INLINE BruterInt   (*bruter_fast_remove_function(BruterList *list, BruterInt i))(BruterList *context, BruterList *args);
// swap two values in the list at indices i1 and i2
STATIC_INLINE void          bruter_swap(BruterList *list, BruterInt i1, BruterInt i2);
// find the index of a key in the list, returns -1 if not found
STATIC_INLINE BruterInt     bruter_find_key(const BruterList *list, const char* key);
// find the index of a value in the list, returns -1 if not found
STATIC_INLINE BruterInt     bruter_find(const BruterList *list, BruterValue value);
STATIC_INLINE BruterInt     bruter_find_int(const BruterList *list, BruterInt value);
STATIC_INLINE BruterInt     bruter_find_uint(const BruterList *list, BruterUInt value);
STATIC_INLINE BruterInt     bruter_find_float(const BruterList *list, BruterFloat value);
STATIC_INLINE BruterInt     bruter_find_pointer(const BruterList *list, void *value);
STATIC_INLINE BruterInt     bruter_find_function(const BruterList *list, BruterInt (*func)(BruterList *context, BruterList *args));
// reverse the list in place
STATIC_INLINE void          bruter_reverse(BruterList *list);
// create a copy of the list, with the same capacity and size, but new data array
STATIC_INLINE BruterList*   bruter_copy(const BruterList *list);
// concatenate two lists, appending src to dest, resizing dest if necessary
STATIC_INLINE void          bruter_concat(BruterList *dest, const BruterList *src);
// receive a context and a list of indexes relative to the context, and call it as a stack
// the function pointer must have the same type as bruter_call BruterInt(*)(BruterList*, BruterList*);
STATIC_INLINE BruterInt     bruter_call(BruterList *context, BruterList *list);
// get a value at index i in the list, returns a value with i set to -1 if index is out of range
STATIC_INLINE BruterValue   bruter_get(const BruterList *list, BruterInt i);
STATIC_INLINE BruterInt     bruter_get_int(const BruterList *list, BruterInt i);
STATIC_INLINE BruterUInt    bruter_get_uint(const BruterList *list, BruterInt i);
STATIC_INLINE BruterFloat   bruter_get_float(const BruterList *list, BruterInt i);
STATIC_INLINE void*         bruter_get_pointer(const BruterList *list, BruterInt i);
STATIC_INLINE BruterInt   (*bruter_get_function(const BruterList *list, BruterInt i))(BruterList *context, BruterList *args);
// set a value at index i in the list, if index is out of range, it will print an error and exit
STATIC_INLINE void          bruter_set(BruterList *list, BruterInt i, BruterValue value, const char* key, int8_t type);
STATIC_INLINE void          bruter_set_int(BruterList *list, BruterInt i, BruterInt value, const char* key, int8_t type);
STATIC_INLINE void          bruter_set_uint(BruterList *list, BruterInt i, BruterUInt value, const char* key, int8_t type);
STATIC_INLINE void          bruter_set_float(BruterList *list, BruterInt i, BruterFloat value, const char* key, int8_t type);
STATIC_INLINE void          bruter_set_pointer(BruterList *list, BruterInt i, void *value, const char* key, int8_t type);
STATIC_INLINE void          bruter_set_function(BruterList *list, BruterInt i, BruterInt (*func)(BruterList *context, BruterList *args), const char* key, int8_t type);
// get a key at index i in the list, returns NULL if index is out of range or if the list is not a table
STATIC_INLINE char*         bruter_get_key(const BruterList *list, BruterInt i);
// set a key at index i in the list, if index is out of range, it will print an error and exit
STATIC_INLINE void          bruter_set_key(const BruterList *list, BruterInt i , const char *key);
// get a type at index i in the list, returns 0 if index is out of range or if the list is not typed
STATIC_INLINE int8_t        bruter_get_type(const BruterList *list, BruterInt i);
// set a type at index i in the list, if index is out of range, it will print an error and exit
STATIC_INLINE void          bruter_set_type(const BruterList *list, BruterInt i, int8_t type);
// get the bruter version
STATIC_INLINE const char*   bruter_get_version(void);

// functions implementations
// functions implementations
// functions implementations
// functions implementations
#ifndef BRUTER_AS_HEADER // you can define this to not include the implementations
BRUTER_ALLOW_AGGREGATE_RETURN()
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

STATIC_INLINE BruterValue bruter_value_function(BruterInt (*func)(BruterList *context, BruterList *args))
{
    return (BruterValue){.fn = func};
}
BRUTER_FORBID_AGGREGATE_RETURN()

STATIC_INLINE BruterList *bruter_new(BruterInt size, bool is_table, bool is_typed)
{
    BruterList *list = (BruterList*)malloc(sizeof(BruterList));
    
    if (unlikely(list == NULL))
    {
        printf("BRUTER_ERROR: failed to allocate memory for BruterList\n");
        exit(EXIT_FAILURE);
    }
    
    list->data = (unlikely(size == 0)) ? NULL : (BruterValue*)malloc((size_t)size * sizeof(BruterValue));
    
    if (likely(size > 0) && unlikely(list->data == NULL))
    {
        printf("BRUTER_ERROR: failed to allocate memory for BruterList data\n");
        free(list);
        exit(EXIT_FAILURE);
    }
    
    list->size = 0;
    list->capacity = size;

    if (is_table)
    {
        list->keys = (char**)calloc((size_t)size, sizeof(char*)); // we need all keys to be NULL
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
        list->types = (int8_t*)calloc((size_t)size, sizeof(int8_t)); // we need all types to be 0
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
    BruterValue *new_data = NULL;
    list->capacity = list->capacity == 0 ? 1 : list->capacity * 2;
    new_data = (BruterValue*)realloc(list->data, (size_t)list->capacity * sizeof(BruterValue));
    if (unlikely(!new_data))
    {
        printf("BRUTER_ERROR: failed to reallocate list data\n");
        exit(EXIT_FAILURE);
    }
    list->data = new_data;

    if (list->keys != NULL)
    {
        char **new_keys = (char**)realloc(list->keys, (size_t)list->capacity * sizeof(char*));
        if (unlikely(!new_keys))
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
    if (unlikely(!new_data))
    {
        printf("BRUTER_ERROR: failed to reallocate list data\n");
        exit(EXIT_FAILURE);
    }
    list->data = new_data;

    if (list->keys != NULL)
    {
        char **new_keys = (char**)realloc(list->keys, (size_t)list->capacity * sizeof(char*));
        if (unlikely(!new_keys))
        {
            printf("BRUTER_ERROR: failed to reallocate list keys\n");
            exit(EXIT_FAILURE);
        }
        list->keys = new_keys;
    }

    if (list->types != NULL)
    {
        int8_t *new_types = (int8_t*)realloc(list->types, (size_t)list->capacity * sizeof(int8_t));
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

STATIC_INLINE void bruter_push_function(BruterList *list, BruterInt (*func)(BruterList *context, BruterList *args), const char* key, int8_t type)
{
    bruter_push(list, (BruterValue){.fn = func}, key, type);
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

STATIC_INLINE void bruter_unshift_function(BruterList *list, BruterInt (*func)(BruterList *context, BruterList *args), const char* key, int8_t type)
{
    bruter_unshift(list, (BruterValue){.fn = func}, key, type);
}

STATIC_INLINE void bruter_insert(BruterList *list, BruterInt i, BruterValue value, const char* key, int8_t type)
{
    if (unlikely(list->size == list->capacity))
    {
        bruter_double(list);
    }

    if (unlikely(i < 0 || i > list->size))
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

STATIC_INLINE void bruter_insert_function(BruterList *list, BruterInt i, BruterInt (*func)(BruterList *context, BruterList *args), const char* key, int8_t type)
{
    bruter_insert(list, i, (BruterValue){.fn = func}, key, type);
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

BRUTER_ALLOW_AGGREGATE_RETURN()
STATIC_INLINE BruterValue bruter_pop(BruterList *list)
{
    BRUTER_FUNCTION_CONTENT_POP();
    return ret;
}
BRUTER_FORBID_AGGREGATE_RETURN()

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

STATIC_INLINE BruterInt (*bruter_pop_function(BruterList *list))(BruterList *context, BruterList *args)
{
    BRUTER_FUNCTION_CONTENT_POP();
    return ret.fn;
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
            memmove(&(list->data[0]), &(list->data[1]), (size_t)(list->size - 1) * sizeof(BruterValue)); \
        } \
        if (list->keys != NULL) \
        { \
            free(list->keys[0]); \
            list->keys[0] = NULL; \
            if (likely(list->size > 1)) \
            { \
                memmove(&(list->keys[0]), &(list->keys[1]), (size_t)(list->size - 1) * sizeof(char*)); \
            } \
        } \
        if (list->types != NULL) \
        { \
            list->types[0] = 0; /* reset type to 0 */ \
            if (likely(list->size > 1)) \
            { \
                memmove(&(list->types[0]), &(list->types[1]), (size_t)(list->size - 1) * sizeof(int8_t)); \
            } \
        } \
        list->size--; \
    } while(0)

BRUTER_ALLOW_AGGREGATE_RETURN()
STATIC_INLINE BruterValue bruter_shift(BruterList *list)
{
    BRUTER_FUNCTION_CONTENT_SHIFT();
    return ret;
}
BRUTER_FORBID_AGGREGATE_RETURN()

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

STATIC_INLINE BruterInt (*bruter_shift_function(BruterList *list))(BruterList *context, BruterList *args)
{
    BRUTER_FUNCTION_CONTENT_SHIFT();
    return ret.fn;
}

#define BRUTER_FUNCTION_CONTENT_REMOVE() \
    BruterValue ret = list->data[i]; \
    do { \
        size_t elements_to_move = 0; \
        if (unlikely(list->size == 0))\
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

BRUTER_ALLOW_AGGREGATE_RETURN()
STATIC_INLINE BruterValue bruter_remove(BruterList *list, BruterInt i)
{
    BRUTER_FUNCTION_CONTENT_REMOVE();
    return ret;
}
BRUTER_FORBID_AGGREGATE_RETURN()

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

STATIC_INLINE BruterInt (*bruter_remove_function(BruterList *list, BruterInt i))(BruterList *context, BruterList *args)
{
    BRUTER_FUNCTION_CONTENT_REMOVE();
    return ret.fn;
}

BRUTER_ALLOW_AGGREGATE_RETURN()
STATIC_INLINE BruterValue bruter_fast_remove(BruterList *list, BruterInt i)
{
    bruter_swap(list, i, list->size - 1);
    return bruter_pop(list);
}
BRUTER_FORBID_AGGREGATE_RETURN()

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

STATIC_INLINE BruterInt (*bruter_fast_remove_function(BruterList *list, BruterInt i))(BruterList *context, BruterList *args)
{
    bruter_swap(list, i, list->size - 1);
    return bruter_pop_function(list);
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

STATIC_INLINE BruterInt bruter_find_function(const BruterList *list, BruterInt (*func)(BruterList *context, BruterList *args))
{
    for (BruterInt i = 0; i < list->size; i++)
    {
        if (list->data[i].fn == func)
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
    
    if (unlikely(copy == NULL))
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
                copy->keys[i] = (char*)malloc(len + 1);
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
        copy->types = (int8_t*)malloc((size_t)copy->capacity * sizeof(int8_t));
        if (unlikely(copy->types == NULL))
        {
            printf("BRUTER_ERROR: failed to allocate memory for BruterList types copy\n");
            bruter_free(copy);
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

STATIC_INLINE void bruter_concat(BruterList *dest, const BruterList *src)
{
    if (dest->size + src->size > dest->capacity)
    {
        while (dest->size + src->size > dest->capacity)
        {
            bruter_double(dest);
        }
    }
    
    memcpy(&(dest->data[dest->size]), src->data, (size_t)src->size * sizeof(BruterValue));

    if (dest->keys != NULL && src->keys != NULL)
    {
        for (BruterInt i = 0; i < src->size; i++)
        {
            if (src->keys[i] != NULL)
            {
                size_t len = strlen(src->keys[i]);
                dest->keys[dest->size + i] = (char*)malloc(len + 1);
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
        memcpy(&(dest->types[dest->size]), src->types, (size_t)src->size * sizeof(int8_t));
    }
    else if (dest->types != NULL)
    {
        memset(&(dest->types[dest->size]), 0, (size_t)src->size * sizeof(int8_t)); // fill with 0 if types are not present in src
    }

    dest->size += src->size;
}

// contextual call
STATIC_INLINE BruterInt bruter_call(BruterList *context, BruterList *list)
{
    BruterInt(*func)(BruterList*, BruterList*);

    if (unlikely(list->size == 0))
    {
        printf("BRUTER_ERROR: cannot call an empty list\n");
        exit(EXIT_FAILURE);
    }

    func = context->data[list->data[0].i].fn;
    return func(context, list);
}

BRUTER_ALLOW_AGGREGATE_RETURN()
STATIC_INLINE BruterValue bruter_get(const BruterList *list, BruterInt i)
{
    if (unlikely(i < 0 || i >= list->size))
    {
        return (BruterValue){.i=-1}; // return -1 if index is out of range
    }
    return list->data[i];
}
BRUTER_FORBID_AGGREGATE_RETURN()

STATIC_INLINE BruterInt bruter_get_int(const BruterList *list, BruterInt i)
{
    if (unlikely(i < 0 || i >= list->size))
    {
        return -1; // return -1 if index is out of range
    }
    return list->data[i].i;
}

STATIC_INLINE BruterUInt bruter_get_uint(const BruterList *list, BruterInt i)
{
    if (unlikely(i < 0 || i >= list->size))
    {
        return 0; // return 0 if index is out of range
    }
    return list->data[i].u;
}

STATIC_INLINE BruterFloat bruter_get_float(const BruterList *list, BruterInt i)
{
    if (unlikely(i < 0 || i >= list->size))
    {
        return (BruterFloat)0.0; // return 0.0 if index is out of range
    }
    return list->data[i].f;
}

STATIC_INLINE void* bruter_get_pointer(const BruterList *list, BruterInt i)
{
    if (unlikely(i < 0 || i >= list->size))
    {
        return NULL; // return NULL if index is out of range
    }
    return list->data[i].p;
}

STATIC_INLINE BruterInt (*bruter_get_function(const BruterList *list, BruterInt i))(BruterList *context, BruterList *args)
{
    if (unlikely(i < 0 || i >= list->size))
    {
        return NULL; // return NULL if index is out of range
    }
    return list->data[i].fn;
}

STATIC_INLINE void bruter_set(BruterList *list, BruterInt i, BruterValue value, const char* key, int8_t type)
{
    if (unlikely(i < 0 || i >= list->size))
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

STATIC_INLINE void bruter_set_function(BruterList *list, BruterInt i, BruterInt (*func)(BruterList *context, BruterList *args), const char* key, int8_t type)
{
    bruter_set(list, i, (BruterValue){.fn = func}, key, type);
}

STATIC_INLINE char* bruter_get_key(const BruterList *list, BruterInt i)
{
    if (list->keys == NULL)
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

STATIC_INLINE void bruter_set_key(const BruterList *list, BruterInt i, const char *key)
{
    if (list->keys == NULL)
    {
        printf("BRUTER_ERROR: bruter_set_key called on a list that is not a table\n");
        exit(EXIT_FAILURE);
    }
    
    if (unlikely(i < 0 || i >= list->size))
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

STATIC_INLINE void bruter_set_type(const BruterList *list, BruterInt i, int8_t type)
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

STATIC_INLINE const char* bruter_get_version(void)
{
    return BRUTER_VERSION;
}
#endif // BRUTER_AS_HEADER macro

// dont expect this to work on restricted compilers and environments, but ofc if can work if has the _Generic feature
// if you wanna mantain maximum compatibility and portability avoid this
// this require either
// a C11 standard and a gcc-compatible compiler
// OR
// a C23 compiler, which standardized the typeof and some __VA_ARGS__ behaviors which were only available as gcc extensions
#ifdef MACRO_BRUTER

    // how we do optional arguments
    #if __STDC_VERSION__ >= 202311L  // C23 
        #define BRUTER_COMMA(...) __VA_OPT__(,)
        #define BRUTER_ARGS(...) 0 __VA_OPT__(,) __VA_ARGS__
    #else // gcc extension
        #define BRUTER_COMMA(...) ,
        #define BRUTER_ARGS(...) 0, ##__VA_ARGS__
    #endif

    // bnew
    // bnew
    // ???N
    // abc3210
    // List bnew(opt int capacity, opt bool istable, opt bool istyped)
    #define bnew_impl_0(dummy, ...) bruter_new(sizeof(BruterValue), false, false)
    #define bnew_impl_1(dummy, capacity, ...) bruter_new(capacity, false, false)
    #define bnew_impl_2(dummy, capacity, istable, ...) bruter_new(capacity, istable, false)
    #define bnew_impl_3(dummy, capacity, keys, istyped, ...) bruter_new(capacity, istable, istyped)
    #define bnew_impl(dummy, _3, _2, _1, N, ...) N 
    #define bnew(...) bnew_impl(BRUTER_ARGS(__VA_ARGS__), bnew_impl_3, bnew_impl_2, bnew_impl_1, bnew_impl_0)(BRUTER_ARGS(__VA_ARGS__))


    // bpush
    // bpush
    // ..??N
    // ..ab432
    // void bpush(List list, Value value, opt char* key, opt int8_t type)
    #define bpush_base(list, value, key, type) \
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
    )(list, value, key, type)
    #define bpush_impl_2(dummy, list, value, ...) bpush_base(list, value, NULL, 0)
    #define bpush_impl_3(dummy, list, value, key, ...) bpush_base(list, value, key, 0)
    #define bpush_impl_4(dummy, list, value, key, type, ...) bpush_base(list, value, key, type)
    #define bpush_impl(dummy, _4, _3, N, ...) N 
    #define bpush(...) bpush_impl(BRUTER_ARGS(__VA_ARGS__), bpush_impl_4, bpush_impl_3, bpush_impl_2)(0, __VA_ARGS__)
    

    // bunshift
    // bunshift
    // ..??N
    // ..ab432
    // void bunshift(List list, Value value, opt char* key, opt int8_t type)
    #define bunshift_base(list, value, key, type) \
    _Generic((value), \
        uint8_t: bruter_unshift_uint, \
        uint16_t: bruter_unshift_uint, \
        uint32_t: bruter_unshift_uint, \
        uint64_t: bruter_unshift_uint, \
        float: bruter_unshift_float, \
        double: bruter_unshift_float, \
        void*: bruter_unshift_pointer, \
        char*: bruter_unshift_pointer, \
        BruterValue: bruter_unshift, \
        default: bruter_unshift_int \
    )(list, value, key, type)
    #define bunshift_impl_2(dummy, list, value, ...) bunshift_base(list, value, NULL, 0)
    #define bunshift_impl_3(dummy, list, value, key, ...) bunshift_base(list, value, key, 0)
    #define bunshift_impl_4(dummy, list, value, key, type, ...) bunshift_base(list, value, key, type)
    #define bunshift_impl(dummy, _4, _3, N, ...) N 
    #define bunshift(...) bunshift_impl(BRUTER_ARGS(__VA_ARGS__), bunshift_impl_4, bunshift_impl_3, bunshift_impl_2)(0, __VA_ARGS__)

    // binsert
    // binsert
    // ...??N
    // ...ab432
    // void binsert(List list, Int index, Value value, opt char* key, opt int8_t type)
    #define binsert_base(list, index, value, key, type) \
    _Generic((value), \
        uint8_t: bruter_insert_uint, \
        uint16_t: bruter_insert_uint, \
        uint32_t: bruter_insert_uint, \
        uint64_t: bruter_insert_uint, \
        float: bruter_insert_float, \
        double: bruter_insert_float, \
        void*: bruter_insert_pointer, \
        char*: bruter_insert_pointer, \
        BruterValue: bruter_insert, \
        default: bruter_insert_int \
    )(list, index, value, key, type)
    #define binsert_impl_3(dummy, list, index, value, ...) binsert_base(list, index, value, NULL, 0)
    #define binsert_impl_4(dummy, list, index, value, key, ...) binsert_base(list, index, value, key, 0)
    #define binsert_impl_5(dummy, list, index, value, key, type, ...) binsert_base(list, index, value, key, type)
    #define binsert_impl(dummy, _5, _4, N, ...) N 
    #define binsert(...) binsert_impl(BRUTER_ARGS(__VA_ARGS__), binsert_impl_5, binsert_impl_4, binsert_impl_3)(0, __VA_ARGS__)
    
    // bset
    // bset
    // ...??N
    // ...ab432
    // void bset(List list, Int index, Value value, opt char* key, opt int8_t type)
    #define bset_base(list, index, value, key, type) \
    _Generic((value), \
        uint8_t: bruter_set_uint, \
        uint16_t: bruter_set_uint, \
        uint32_t: bruter_set_uint, \
        uint64_t: bruter_set_uint, \
        float: bruter_set_float, \
        double: bruter_set_float, \
        void*: bruter_set_pointer, \
        char*: bruter_set_pointer, \
        BruterValue: bruter_set, \
        default: bruter_set_int \
    )(list, index, value, key, type)
    #define bset_impl_3(dummy, list, index, value, ...) bset_base(list, index, value, NULL, 0)
    #define bset_impl_4(dummy, list, index, value, key, ...) bset_base(list, index, value, key, 0)
    #define bset_impl_5(dummy, list, index, value, key, type, ...) bset_base(list, index, value, key, type)
    #define bset_impl(dummy, _5, _4, N, ...) N 
    #define bset(...) bset_impl(BRUTER_ARGS(__VA_ARGS__), bset_impl_5, bset_impl_4, bset_impl_3)(0, __VA_ARGS__)

    // macros bellow doesnt have any optional args
    #define bfree bruter_free
    #define bdouble bruter_double
    #define bhalf bruter_half
    #define bpop bruter_pop
    #define bshift bruter_shift
    #define bremove bruter_remove
    #define bfast_remove bruter_fast_remove
    #define bswap bruter_swap
    #define bfind_key bruter_find_key
    #define breverse bruter_reverse
    #define bcopy bruter_copy
    #define bconcat bruter_concat
    #define bcall bruter_call
    #define bget bruter_get
    #define bget_key bruter_get_key
    #define bset_key bruter_set_key
    #define bget_type bruter_get_type
    #define bset_type bruter_set_type
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
        BruterValue: bruter_find, \
        default: bruter_find_int \
    )(list, value)
    // this generate aggregate return warning
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

    // supress unused macro warnings
    #if defined(BRUTER_AS_SOURCE)
        #if defined(bnew)
            #if defined(bpush) && defined(bunshift) && defined(binsert) && defined(bset) && \
                defined(bfree) && defined(bdouble) && defined(bhalf) && defined(bpop) && \
                defined(bshift) && defined(bremove) && defined(bfast_remove) && defined(bswap) && \
                defined(bfind_key) && defined(breverse) && defined(bcopy) && defined(bconcat) && \
                defined(bcall) && defined(bget) && defined(bget_key) && defined(bset_key) && \
                defined(bget_type) && defined(bset_type)
                #if defined(bfind) && defined(bvalue)
                    #if defined(binsert_base) && defined(bset_base) && \
                        defined(bpush_base) && defined(bunshift_base)
                        #if defined(bnew_impl) && defined(bpush_impl) && \
                            defined(bunshift_impl) && defined(binsert_impl) && defined(bset_impl)
                            #if defined(bnew_impl_0) && defined(bpush_impl_2) && \
                                defined(bunshift_impl_2) && defined(binsert_impl_3) && defined(bset_impl_3)
                                #if defined(bpush_impl_3) && defined(bunshift_impl_3) && \
                                    defined(binsert_impl_4) && defined(bset_impl_4)
                                    #if defined(bnew_impl_1) && defined(bnew_impl_2) && defined(bnew_impl_3)
                                        #if defined(bpush_impl_4) && defined(bunshift_impl_4) && \
                                            defined(binsert_impl_5) && defined(bset_impl_5)
                                            #if defined(BRUTER_COMMA) && defined(BRUTER_ARGS)
                                                // just to avoid warnings about unused macros
                                            #endif
                                        #endif
                                    #endif
                                #endif
                            #endif
                        #endif
                    #endif
                #endif
            #endif
        #endif
    #endif
#endif

// uncomment next line to force disable C++ wrapper
// #define BRUTER_CPP 1

// uncomment next line to force enable C++ wrapper
// #define BRUTER_CPP 0

// C++ wrapper for BruterList
// This is only compiled if BRUTER_CPP is defined, which is usually done automatically
// you can also define it manually
// the wrapper is not garanteed to meet any C++ standard, i know little to nothing about C++ standard
// might not be as fast as C code, i know nothing about C++ optimization either
// might not be as clean and readable as C code, bcause C++ can be very messy sometimes
// i only focused on making it easy to use, comprehensible and somewhat readable;
#if BRUTER_CPP
// typedef is redundant but we use it just to standardize like in C code
// inline in methods are also redundant, but we use it to standardize like is in C code
typedef class Bruter 
{
    private:
        BruterList* list;
    public:
        // Bruter::VERSION, just accessible as a class member(not an object member)
        inline static constexpr const char* VERSION = BRUTER_VERSION;

        // constructor
        inline Bruter(int capacity = 8, bool keyless = false, bool typeless = false);

        // destructor
        inline ~Bruter();
        
        // indexing operator
        inline BruterValue&    operator[](BruterInt i);
        
        // capacity management
        inline void            double_capacity();
        inline void            half_capacity();
        
        // push overloads
        inline void            push(BruterValue value, const char* key = NULL, int8_t type = 0);
        inline void            push(BruterInt value, const char* key = NULL, int8_t type = 0);
        inline void            push(BruterUInt value, const char* key = NULL, int8_t type = 0);
        inline void            push(BruterFloat value, const char* key = NULL, int8_t type = 0);
        inline void            push(void* value, const char* key = NULL, int8_t type = 0);
        inline void            push(BruterInt (*func)(BruterList *context, BruterList *args), const char* key = NULL, int8_t type = 0);
        
        // unshift overloads
        inline void            unshift(BruterValue value, const char* key = NULL, int8_t type = 0);
        inline void            unshift(BruterInt value, const char* key = NULL, int8_t type = 0);
        inline void            unshift(BruterUInt value, const char* key = NULL, int8_t type = 0);
        inline void            unshift(BruterFloat value, const char* key = NULL, int8_t type = 0);
        inline void            unshift(void* value, const char* key = NULL, int8_t type = 0);
        inline void            unshift(BruterInt (*func)(BruterList *context, BruterList *args), const char* key = NULL, int8_t type = 0);
        
        // insert overloads
        inline void            insert(BruterInt index, BruterValue value, const char* key = NULL, int8_t type = 0);
        inline void            insert(BruterInt index, BruterInt value, const char* key = NULL, int8_t type = 0);
        inline void            insert(BruterInt index, BruterUInt value, const char* key = NULL, int8_t type = 0);
        inline void            insert(BruterInt index, BruterFloat value, const char* key = NULL, int8_t type = 0);
        inline void            insert(BruterInt index, void* value, const char* key = NULL, int8_t type = 0);
        inline void            insert(BruterInt index, BruterInt (*func)(BruterList *context, BruterList *args), const char* key = NULL, int8_t type = 0);
        
        // regular methods declarations
        inline BruterValue     pop();
        inline BruterValue     shift();
        inline BruterValue     remove(BruterInt i);
        inline BruterValue     fast_remove(BruterInt i);
        inline void            swap(BruterInt i1, BruterInt i2);
        inline BruterInt       find_key(const char* key) const;
        
        // find overloads
        inline BruterInt       find(BruterValue value) const;
        inline BruterInt       find(BruterInt value) const;
        inline BruterInt       find(BruterUInt value) const;
        inline BruterInt       find(BruterFloat value) const;
        inline BruterInt       find(void* value) const;
        inline BruterInt       find(BruterInt (*func)(BruterList *context, BruterList *args)) const;
        
        // regular methods declarations
        inline void            reverse();
        inline BruterList*     copy() const;
        inline void            concat(const BruterList* src);
        inline BruterInt       call(BruterList* cmd);
        inline BruterValue     get(BruterInt i) const;
        
        // set overloads
        inline void            set(BruterInt i, BruterValue value, const char* key = NULL, int8_t type = 0);
        inline void            set(BruterInt i, BruterInt value, const char* key = NULL, int8_t type = 0);
        inline void            set(BruterInt i, BruterUInt value, const char* key = NULL, int8_t type = 0);
        inline void            set(BruterInt i, BruterFloat value, const char* key = NULL, int8_t type = 0);
        inline void            set(BruterInt i, void* value, const char* key = NULL, int8_t type = 0);
        inline void            set(BruterInt i, BruterInt (*func)(BruterList *context, BruterList *args), const char* key = NULL, int8_t type = 0);
        
        // key and type getters and setters
        inline const char*     get_key(BruterInt i) const;
        inline void            set_key(BruterInt i, const char* key);
        inline int8_t          get_type(BruterInt i) const;
        inline void            set_type(BruterInt i, int8_t type);
        
        // C++ specific methods
        inline void            clear();
        
        // C++ specific getters(list is private)
        inline BruterInt       size() const;
        inline BruterInt       capacity() const;
        inline bool            is_table() const;
        inline bool            is_typed() const;
        inline BruterList*     get_list() const;
} Bruter;

// Bruter class methods implementation
// this is the indexing operator
inline BruterValue& Bruter::operator[](BruterInt i) 
{
    if (unlikely(i < 0 || i >= list->size))
    {
        printf("BRUTER_ERROR: index %" PRIdPTR " out of range in list of size %" PRIdPTR ".\n", i, list->size);
        exit(EXIT_FAILURE);
    }

    // return the reference to the value at index i
    // this allows us to use the index operator to access and modify elements
    // e.g. list[i] = value;
    // or value = list[i];
    // or even list[i].i = 42; to modify the int value directly
    // this is fun, C++ can be fun sometimes
    return list->data[i];
}

// constructor
inline Bruter::Bruter(int capacity = 8, bool keyless = false, bool typeless = false) 
{
    list = bruter_new(capacity, keyless, typeless);
}

// destructor
inline Bruter::~Bruter() 
{
    bruter_free(list);
}

// double the capacity of the list
inline void Bruter::double_capacity() 
{
    bruter_double(list);
}

// half the capacity of the list
inline void Bruter::half_capacity() 
{
    bruter_half(list);
}

// push different types of values
// this is the default push method
inline void Bruter::push(BruterValue value, const char* key = NULL, int8_t type = 0) 
{
    bruter_push(list, value, key, type);
}

// push int overload
inline void Bruter::push(BruterInt value, const char* key = NULL, int8_t type = 0) 
{
    bruter_push_int(list, value, key, type);
}

// push uint overload
inline void Bruter::push(BruterUInt value, const char* key = NULL, int8_t type = 0) 
{
    bruter_push_uint(list, value, key, type);
}

// push float overload
inline void Bruter::push(BruterFloat value, const char* key = NULL, int8_t type = 0) 
{
    bruter_push_float(list, value, key, type);
}

// push pointer overload
inline void Bruter::push(void* value, const char* key = NULL, int8_t type = 0) 
{
    bruter_push_pointer(list, value, key, type);
}

// push function overload
inline void Bruter::push(BruterInt (*func)(BruterList *context, BruterList *args), const char* key = NULL, int8_t type = 0) 
{
    bruter_push_function(list, func, key, type);
}

// unshift different types of values
// this is the default unshift method
inline void Bruter::unshift(BruterValue value, const char* key = NULL, int8_t type = 0) 
{
    bruter_unshift(list, value, key, type);
}

// unshift int overload
inline void Bruter::unshift(BruterInt value, const char* key = NULL, int8_t type = 0) 
{
    bruter_unshift_int(list, value, key, type);
}

// unshift uint overload
inline void Bruter::unshift(BruterUInt value, const char* key = NULL, int8_t type = 0) 
{
    bruter_unshift_uint(list, value, key, type);
}

// unshift float overload
inline void Bruter::unshift(BruterFloat value, const char* key = NULL, int8_t type = 0) 
{
    bruter_unshift_float(list, value, key, type);
}

// unshift pointer overload
inline void Bruter::unshift(void* value, const char* key = NULL, int8_t type = 0) 
{
    bruter_unshift_pointer(list, value, key, type);
}

// unshift function overload
inline void Bruter::unshift(BruterInt (*func)(BruterList *context, BruterList *args), const char* key = NULL, int8_t type = 0) 
{
    bruter_unshift_function(list, func, key, type);
}

// insert different types of values at a specific index
// this is the default insert method
inline void Bruter::insert(BruterInt index, BruterValue value, const char* key = NULL, int8_t type = 0) 
{
    bruter_insert(list, index, value, key, type);
}

// insert int overload
inline void Bruter::insert(BruterInt index, BruterInt value, const char* key = NULL, int8_t type = 0) 
{
    bruter_insert_int(list, index, value, key, type);
}

// insert uint overload
inline void Bruter::insert(BruterInt index, BruterUInt value, const char* key = NULL, int8_t type = 0) 
{
    bruter_insert_uint(list, index, value, key, type);
}

// insert float overload
inline void Bruter::insert(BruterInt index, BruterFloat value, const char* key = NULL, int8_t type = 0) 
{
    bruter_insert_float(list, index, value, key, type);
}

// insert pointer overload
inline void Bruter::insert(BruterInt index, void* value, const char* key = NULL, int8_t type = 0) 
{
    bruter_insert_pointer(list, index, value, key, type);
}

// insert function overload
inline void Bruter::insert(BruterInt index, BruterInt (*func)(BruterList *context, BruterList *args), const char* key = NULL, int8_t type = 0) 
{
    bruter_insert_function(list, index, func, key, type);
}

// pop
inline BruterValue Bruter::pop() 
{
    return bruter_pop(list);
}

// shift
inline BruterValue Bruter::shift() 
{
    return bruter_shift(list);
}

// remove
inline BruterValue Bruter::remove(BruterInt i) 
{
    return bruter_remove(list, i);
}

// fast remove
inline BruterValue Bruter::fast_remove(BruterInt i) 
{
    return bruter_fast_remove(list, i);
}

// swap two elements
inline void Bruter::swap(BruterInt i1, BruterInt i2) 
{
    bruter_swap(list, i1, i2);
}

// find a index by key
inline BruterInt Bruter::find_key(const char* key) const 
{
    return bruter_find_key(list, key);
}

// find a value by different types
// this is the default find method
inline BruterInt Bruter::find(BruterValue value) const 
{
    return bruter_find(list, value);
}

// find int overload
inline BruterInt Bruter::find(BruterInt value) const 
{
    return bruter_find_int(list, value);
}

// find uint overload
inline BruterInt Bruter::find(BruterUInt value) const 
{
    return bruter_find_uint(list, value);
}

// find float overload
inline BruterInt Bruter::find(BruterFloat value) const 
{
    return bruter_find_float(list, value);
}

// find pointer overload
inline BruterInt Bruter::find(void* value) const 
{
    return bruter_find_pointer(list, value);
}

// find function overload
inline BruterInt Bruter::find(BruterInt (*func)(BruterList *context, BruterList *args)) const 
{
    return bruter_find_function(list, func);
}

// reverse the list
inline void Bruter::reverse() 
{
    bruter_reverse(list);
}

// copy the list
inline BruterList* Bruter::copy() const 
{
    return bruter_copy(list);
}

// concatenate another list
inline void Bruter::concat(const BruterList* src) 
{
    bruter_concat(list, src);
}

// contextual call
// the context is the list itself
inline BruterInt Bruter::call(BruterList* cmd) 
{
    return bruter_call(list, cmd);
}

// get, prefer to use operator[]
inline BruterValue Bruter::get(BruterInt i) const 
{
    return bruter_get(list, i);
}

// set, prefer to use operator[]
// this is the default set method
inline void Bruter::set(BruterInt i, BruterValue value, const char* key = NULL, int8_t type = 0) 
{
    bruter_set(list, i, value, key, type);
}

// set int overload
inline void Bruter::set(BruterInt i, BruterInt value, const char* key = NULL, int8_t type = 0) 
{
    bruter_set_int(list, i, value, key, type);
}

// set uint overload
inline void Bruter::set(BruterInt i, BruterUInt value, const char* key = NULL, int8_t type = 0) 
{
    bruter_set_uint(list, i, value, key, type);
}

// set float overload
inline void Bruter::set(BruterInt i, BruterFloat value, const char* key = NULL, int8_t type = 0) 
{
    bruter_set_float(list, i, value, key, type);
}

// set pointer overload
inline void Bruter::set(BruterInt i, void* value, const char* key = NULL, int8_t type = 0) 
{
    bruter_set_pointer(list, i, value, key, type);
}

// set function overload
inline void Bruter::set(BruterInt i, BruterInt (*func)(BruterList *context, BruterList *args), const char* key = NULL, int8_t type = 0) 
{
    bruter_set_function(list, i, func, key, type);
}

// get key
// same as get_key in C
inline const char* Bruter::get_key(BruterInt i) const 
{
    return bruter_get_key(list, i);
}

// set key
// same as set_key in C
inline void Bruter::set_key(BruterInt i, const char* key) 
{
    bruter_set_key(list, i, key);
}

// get type
// same as get_type in C
inline int8_t Bruter::get_type(BruterInt i) const 
{
    return bruter_get_type(list, i);
}

// set type
// same as set_type in C
inline void Bruter::set_type(BruterInt i, int8_t type) 
{
    bruter_set_type(list, i, type);
}

// c++ specific methods
// returns the size of the list
// just because list is private
inline BruterInt Bruter::size() const 
{
    return list->size;
}

// free the list
// then recreate it with the same capacity and keyless/typeless options
inline void Bruter::clear() 
{
    bool is_table = list->keys != NULL;
    bool is_typed = list->types != NULL;
    bruter_free(list);
    list = bruter_new(list->capacity, is_table, is_typed);
}

// returns the capacity of the list
// just because list is private
// it is the maximum number of elements that can be stored in the list without reallocating
// it is not the same as size, which is the number of elements currently stored in the list
// capacity is always a power of 2, and it is doubled when the list is full
// it is used to optimize memory allocation and avoid fragmentation
inline BruterInt Bruter::capacity() const 
{
    return list->capacity;
}

// returns true if the list is a table (has keys)
// just because list is private
// a table is a list that has keys associated with each element
// it is used to store key-value pairs, like a dictionary or a map
inline bool Bruter::is_table() const 
{
    return list->keys != NULL;
}

// returns true if the list is typed (has types)
// just because list is private
// a typed list is a list that has types associated with each element
// it is used to store elements of different types in a union called BruterValue
// types arent automatically checked or anything like that, no bruter function will check types
// it is up to the user to check types if needed
// use it as pure metadata
inline bool Bruter::is_typed() const 
{
    return list->types != NULL;
}

// returns the internal list pointer
// just because list is private
// this is useful if you want to use the list with other functions that expect a BruterList*
// it is not recommended to use this pointer directly(in C++, if you want so, just stick to bruter)
// it is better to use the methods provided by this class
// i mean, if you using this wrapper then you probably want something safer, easier and higher level than regular C bruter
// if you want to use it that low level, then just use the C API directly
// i mean, just use this method when you really need it
inline BruterList* Bruter::get_list() const 
{
    return list;
}
#endif // this closes the C++ wrapper

// we dont need this in header files or regular bruter
// this is only used if one compiling the bruter as a lib
// else, if you not compiling a lib everything is static inline
#if defined(BRUTER_AS_SOURCE)
    #if defined(BRUTER_H) && defined(BRUTER_VERSION)
        // just to avoid warnings about unused macros
        #if defined(BRUTER_ALLOW_AGGREGATE_RETURN) && defined(BRUTER_FORBID_AGGREGATE_RETURN)
            // just to avoid warnings about unused macros
        #endif

        #if defined(BRUTER_ALLOW_NON_LITERAL_FORMAT) && defined(BRUTER_FORBID_NON_LITERAL_FORMAT)
            // just to avoid warnings about unused macros
        #endif
    #endif
    #if defined(BRUTER_CPP)
        // just to avoid warnings about unused macros
        #if defined(Bruter)
            // just to avoid warnings about unused macros
        #endif
    #endif
#endif

#endif
