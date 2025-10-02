#include "bruter.h"

void feraw_equals(BruterList* stack)
{
    BruterInt a = bruter_pop_int(stack);
    BruterInt b = bruter_pop_int(stack);
    bruter_push_int(stack, a == b ? 1 : 0, NULL, BRUTER_TYPE_ANY);
}

void feraw_not_equals(BruterList* stack)
{
    BruterInt a = bruter_pop_int(stack);
    BruterInt b = bruter_pop_int(stack);
    bruter_push_int(stack, a != b ? 1 : 0, NULL, BRUTER_TYPE_ANY);
}

void feraw_greater(BruterList* stack)
{
    BruterInt a = bruter_pop_int(stack);
    BruterInt b = bruter_pop_int(stack);
    bruter_push_int(stack, a > b ? 1 : 0, NULL, BRUTER_TYPE_ANY);
}

void feraw_greater_equal(BruterList* stack)
{
    BruterInt a = bruter_pop_int(stack);
    BruterInt b = bruter_pop_int(stack);
    bruter_push_int(stack, a >= b ? 1 : 0, NULL, BRUTER_TYPE_ANY);
}

void feraw_less(BruterList* stack)
{
    BruterInt a = bruter_pop_int(stack);
    BruterInt b = bruter_pop_int(stack);
    bruter_push_int(stack, a < b ? 1 : 0, NULL, BRUTER_TYPE_ANY);
}

void feraw_less_equal(BruterList* stack)
{
    BruterInt a = bruter_pop_int(stack);
    BruterInt b = bruter_pop_int(stack);
    bruter_push_int(stack, a <= b ? 1 : 0, NULL, BRUTER_TYPE_ANY);
}

void feraw_and(BruterList* stack)
{
    BruterInt a = bruter_pop_int(stack);
    BruterInt b = bruter_pop_int(stack);
    bruter_push_int(stack, a && b ? 1 : 0, NULL, BRUTER_TYPE_ANY);
}

void feraw_or(BruterList* stack)
{
    BruterInt a = bruter_pop_int(stack);
    BruterInt b = bruter_pop_int(stack);
    bruter_push_int(stack, a || b ? 1 : 0, NULL, BRUTER_TYPE_ANY);
}

void feraw_not(BruterList* stack)
{
    BruterInt a = bruter_pop_int(stack);
    bruter_push_int(stack, !a ? 1 : 0, NULL, BRUTER_TYPE_ANY);
}