#define BRUTER_USE_SHORT_TYPES
#include <bruter.h>

int main(void)
{
    List *list = bnew(8, false, false);
    Int i = 42;
    bpush(list, i, NULL);
    bunshift(list, 3.14f, NULL);
    binsert(list, 1, (void*)list, NULL);
    breverse(list);
    Value v = bpop(list);
    printf("List size: %ld\n", list->size);
    printf("Popped value: %f\n", v.f);
    bfree(list);
    return 0;
}