#include <bruter.h>

int main(void)
{
    BruterList *list = bruter_new(sizeof(BruterValue), true, true);
    BruterInt i = 42;
    bruter_push_int(list, i, NULL, 0);
    bruter_unshift_float(list, 3.14f, NULL, 0);
    bruter_insert_pointer(list, 1, (void*)list, NULL, 0);
    bruter_reverse(list);
    BruterValue v = bruter_pop(list);
    printf("List size: %ld\n", list->size);
    printf("Popped value: %f\n", v.f);
    bruter_free(list);
    return 0;
}
