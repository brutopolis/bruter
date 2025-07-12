
# BRUTER

  BRUTER is a C header-only library to deal with lists of variables, it is designed to be simple and fast while extremely easy to use and understand.

  BRUTER is meant to be 100% C99 compliant;

# Library

  BRUTER is a single header file `bruter.h`, just include and ready to go, there are some macros to manage the implementation, you can define them before including the header file to change the behavior of the library.

# Usage


  ```c
// can be found at example/simple.c
#include <bruter.h>

int main()
{
    BruterList *list = bruter_new(8);
    BruterInt i = 42;
    bruter_push(list, bruter_value_int(i));
    bruter_unshift(list, bruter_value_float(3.14f));
    bruter_insert(list, 1, bruter_value_pointer(list));
    bruter_push(list, bruter_value_pointer(strdup("Hello, World!")));
    bruter_reverse(list);
    BruterValue v = bruter_pop(list);
    printf("List size: %d\n", list->size);
    printf("Popped value: %f\n", v.f);
    free(bruter_shift(list).s);
    bruter_free(list);
    return 0;
}
```
