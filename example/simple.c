#define MACRO_BRUTER 1 // we need to activate macro bruter to use variadic calls
#include <bruter.h>

int main(void)
{
    BruterList *list = bnew();
    BruterInt i = 42;
    bpush(list, i, NULL);
    bunshift(list, 3.14f, NULL);
    binsert(list, 1, (void*)list, NULL);
    breverse(list);
    BruterValue v = bpop(list);
    printf("List size: %ld\n", list->size);
    printf("Popped value: %f\n", v.f);
    bfree(list);
    return 0;
}
