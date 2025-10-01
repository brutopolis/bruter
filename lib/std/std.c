#include "bruter.h"

void feraw_drop(BruterList* stack)
{
    bruter_pop_pointer(stack);
}

void feraw_eval(BruterList* stack)
{
    BruterList* context = bruter_pop_pointer(stack);
    char* code = (char*)bruter_pop_pointer(stack);
    BruterList* splited = bruter_pop_pointer(stack);
    BruterList* prog_stack = bruter_pop_pointer(stack);
    bruter_interpret(context, code, splited, prog_stack);
}
