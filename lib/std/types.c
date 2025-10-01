#include "bruter.h"

void feraw_to_int(BruterList* stack)
{
    BruterMeta meta = bruter_pop_meta(stack);
    if (meta.type == BRUTER_TYPE_FLOAT)
    {
        bruter_push_int(stack, (BruterInt)meta.value.f, NULL, BRUTER_TYPE_ANY);
    }
    else if (meta.type == BRUTER_TYPE_BUFFER)
    {
        bruter_push_int(stack, atoi((char*)meta.value.p), NULL, BRUTER_TYPE_ANY);
    }
    else
    {
        bruter_push_int(stack, meta.value.i, NULL, BRUTER_TYPE_ANY);
    }
}

void feraw_to_float(BruterList* stack)
{
    BruterMeta meta = bruter_pop_meta(stack);
    if (meta.type == BRUTER_TYPE_FLOAT)
    {
        bruter_push_float(stack, meta.value.f, NULL, BRUTER_TYPE_ANY);
    }
    else if (meta.type == BRUTER_TYPE_BUFFER)
    {
        bruter_push_float(stack, atof((char*)meta.value.p), NULL, BRUTER_TYPE_ANY);
    }
    else
    {
        bruter_push_float(stack, (float)meta.value.i, NULL, BRUTER_TYPE_ANY);
    }
}

void feraw_to_string(BruterList* stack)
{
    BruterMeta meta = bruter_pop_meta(stack);
    char buffer[32];
    if (meta.type == BRUTER_TYPE_FLOAT)
    {
        snprintf(buffer, sizeof(buffer), "%f", meta.value.f);
        bruter_push_pointer(stack, strdup(buffer), NULL, BRUTER_TYPE_BUFFER);
    }
    else if (meta.type == BRUTER_TYPE_BUFFER)
    {
        bruter_push_pointer(stack, strdup((char*)meta.value.p), NULL, BRUTER_TYPE_BUFFER);
    }
    else
    {
        snprintf(buffer, sizeof(buffer), "%ld", meta.value.i);
        bruter_push_pointer(stack, strdup(buffer), NULL, BRUTER_TYPE_BUFFER);
    }
}
