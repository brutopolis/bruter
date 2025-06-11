
# BRUTER

  BRUTER is a C header-only library to deal with lists of variables, it is designed to be simple and fast while extremely easy to use and uderstand.

# Concept

  BRUTER have a optional type system, each variable will have a byte for type, you can also disable completely the type system;
  ```c
  #define BRUTER_TYPELESS
  ```

  BRUTER have also a optional key system, each variable will have a a pointer to a key, even if the key is not used, it can be completely disabled defining BRUTER_KEYLESS:
  ```c
  #define BRUTER_KEYLESS
  ```

  Even if the key or the type system is enabled you can choose to not use them on a specific list, just set list.types or list.keys to `NULL`:
  ```c
  list.types = NULL;
  list.keys = NULL;
  ```
