
# BRUTER

  BRUTER is a C header-only library to deal with lists of variables, it is designed to be simple and fast while extremely easy to use and uderstand.

# Library

  BRUTER is a single header file `bruter.h`, just include and ready to go, there are some macros to manage the implementation, you can define them before including the header file to change the behavior of the library.

  ```c
  #define BRUTER_TYPELESS // Disable the type system
  #define BRUTER_KEYLESS // Disable the key system
  #define USE_SHORT_TYPES // Use short type names
  #include <bruter.h>
  ```

  BRUTER have a optional type system, each variable will have a byte for type, you can also disable completely the type system;
  ```c
  #define BRUTER_TYPELESS
  #include <bruter.h>
  ```

  BRUTER have also a optional key system, each variable will have a pointer to a key, even if the key is not used, it can be completely disabled defining BRUTER_KEYLESS:
  ```c
  #define BRUTER_KEYLESS
  #include <bruter.h>
  ```

  BRUTER can also use short type names:

  ```c
  #define USE_SHORT_TYPES
  #include <bruter.h>
  Int i; // short for BruterInt
  UInt u; // short for BruterUInt
  Float f; // short for BruterFloat
  Value v; // short for BruterValue
  List list; // short for BruterList
  ```

  Even if the key or the type system is enabled you can choose to not use them on a specific list, just set list.types or list.keys to `NULL`:
  ```c
  list.types = NULL;
  list.keys = NULL;
  ```

# Usage


  ```c

// can be found at example/simple.c

#define USE_SHORT_TYPES
#define BRUTER_TYPELESS
#define BRUTER_KEYLESS
#include <bruter.h>

int main()
{
    List *list = bruter_init(8);
    Int i = 42;
    bruter_push(list, bruter_value_i(i));
    bruter_unshift(list, bruter_value_f(3.14f));
    bruter_insert(list, 1, bruter_value_p(list));
    bruter_push(list, bruter_value_p(strdup("Hello, World!")));
    bruter_reverse(list);
    Value v = bruter_pop(list);
    printf("List size: %d\n", list->size);
    printf("Popped value: %f\n", v.f);
    free(bruter_shift(list).s);
    bruter_free(list);
    return 0;
}
```
