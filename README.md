
# BRUTER

  BRUTER is a C header-only library to deal with lists of variables, it is designed to be simple and fast while extremely easy to use and understand.

# Library

  BRUTER is a single header file `bruter.h`, just include and ready to go, there are some macros to manage the implementation, you can define them before including the header file to change the behavior of the library.

  ```c
  #define BRUTER_USE_SHORT_TYPES // Use short type names
  #include <bruter.h>
  ```

  BRUTER can also use short type names:

  ```c
  #define BRUTER_USE_SHORT_TYPES
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

#define BRUTER_USE_SHORT_TYPES
#include <bruter.h>

int main()
{
    List *list = bruter_new(8);
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
