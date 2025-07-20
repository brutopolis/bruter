// bruter arena
// bruter arena use no other libraries beside bruter (which only uses the standard C99 libraries)
#ifndef BRUTER_ARENA_H
#define BRUTER_ARENA_H 1

#include <bruter.h>

#define BRUTER_VERSION "0.0.1"

STATIC_INLINE void* bruter_arena_alloc(BruterArena *arena, size_t size)
{
    if (arena->offset + size > arena->capacity)
    {
        bruter_double((struct BruterList*)arena);
    }
    void *ptr = (char*)arena->data + arena->offset;
    arena->offset += size;
    return ptr;
}

#endif // BRUTER_ARENA_H macro