#include <bruter.h>

int main(void)
{
    // lets create a arena
    BruterArena *arena = (BruterArena*)bruter_new(128, false, false);
    
    // lets allocate some
    char *str = (char*)bruter_alloc(arena, sizeof("hello wordasdasdasdadasdadasdld!"));
    
    strcpy(str, "hello wordasdasdasdadasdadasdld!");
    
    printf("Allocated string: %s\n", str);
    printf("Arena capacity: %ld bytes\n", arena->capacity* sizeof(void*));
    printf("Arena offset: %ld\n", arena->offset);
    printf("Arena data: %p\n", arena->data);
    bruter_free((BruterList*)arena);
    return 0;
}