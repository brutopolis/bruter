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