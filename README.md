
# BRUTER

  BRUTER is a C header-only library to deal with lists of variables, it is designed to be simple and fast while extremely easy to use and understand.

  BRUTER is meant to be 100% C99 compliant;

  BRUTER can **optionally** use _Generics(C11), typeof(GCC or C23) and \__VA_OPT__ (C23, but there is a fallback for GCC), with the optional features turned on it can be compiled with any C23 compliant compiler, or any GCC version that supports _Generics;

  BRUTER can **optionally** use a C++ wrapper, C++ code is not 100% compliant to any C++ standard, so, i can only guarantee that it will work with GCC and Clang, any other C++ compiler that might compile it for sure 100% luck 0% planned;

# Library

  BRUTER is a single header file `bruter.h`, just include and ready to go, there are some macros to manage the implementation, you can define them before including the header file to change the behavior of the library.

# C++ Wrapper

  BRUTER automatically detects if you are compiling with a C++ compiler, also, you can force it using the `BRUTER_CPP` macro, it will implement a `Bruter` class that have almost every function from the C API, i dont know much about C++ standards, so its not 100% compliant to anything, maybe the latest C++ standard, but i really dont know, it also might use some GCC extensions;

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
