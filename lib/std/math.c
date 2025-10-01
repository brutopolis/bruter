#include <bruter.h>
#include <math.h>

void feraw_add(BruterList* stack)
{
    BruterMeta a = bruter_pop_meta(stack);
    BruterMeta b = bruter_pop_meta(stack);
    switch (a.type)
    {
        case BRUTER_TYPE_FLOAT:
            switch (b.type)
            {
                case BRUTER_TYPE_FLOAT:
                    bruter_push_float(stack, a.value.f + b.value.f, NULL, BRUTER_TYPE_FLOAT);
                    break;
                default:
                    bruter_push_float(stack, a.value.f + b.value.i, NULL, BRUTER_TYPE_FLOAT);
                    break;
            }
            break;
        default:
            switch (b.type)
            {
                case BRUTER_TYPE_FLOAT:
                    bruter_push_float(stack, a.value.i + b.value.f, NULL, BRUTER_TYPE_FLOAT);
                    break;
                default:
                    bruter_push_int(stack, a.value.i + b.value.i, NULL, BRUTER_TYPE_ANY);
                    break;
            }
            break;
    }
}

void feraw_sub(BruterList* stack)
{
    BruterMeta a = bruter_pop_meta(stack);
    BruterMeta b = bruter_pop_meta(stack);
    switch (a.type)
    {
        case BRUTER_TYPE_FLOAT:
            switch (b.type)
            {
                case BRUTER_TYPE_FLOAT:
                    bruter_push_float(stack, a.value.f - b.value.f, NULL, BRUTER_TYPE_FLOAT);
                    break;
                default:
                    bruter_push_float(stack, a.value.f - b.value.i, NULL, BRUTER_TYPE_FLOAT);
                    break;
            }
            break;
        default:
            switch (b.type)
            {
                case BRUTER_TYPE_FLOAT:
                    bruter_push_float(stack, a.value.i - b.value.f, NULL, BRUTER_TYPE_FLOAT);
                    break;
                default:
                    bruter_push_int(stack, a.value.i - b.value.i, NULL, BRUTER_TYPE_ANY);
                    break;
            }
            break;
    }
}

void feraw_mul(BruterList* stack)
{
    BruterMeta a = bruter_pop_meta(stack);
    BruterMeta b = bruter_pop_meta(stack);
    switch (a.type)
    {
        case BRUTER_TYPE_FLOAT:
            switch (b.type)
            {
                case BRUTER_TYPE_FLOAT:
                    bruter_push_float(stack, a.value.f * b.value.f, NULL, BRUTER_TYPE_FLOAT);
                    break;
                default:
                    bruter_push_float(stack, a.value.f * b.value.i, NULL, BRUTER_TYPE_FLOAT);
                    break;
            }
            break;
        default:
            switch (b.type)
            {
                case BRUTER_TYPE_FLOAT:
                    bruter_push_float(stack, a.value.i * b.value.f, NULL, BRUTER_TYPE_FLOAT);
                    break;
                default:
                    bruter_push_int(stack, a.value.i * b.value.i, NULL, BRUTER_TYPE_ANY);
                    break;
            }
            break;
    }
}

void feraw_div(BruterList* stack)
{
    BruterMeta a = bruter_pop_meta(stack);
    BruterMeta b = bruter_pop_meta(stack);
    switch (a.type)
    {
        case BRUTER_TYPE_FLOAT:
            switch (b.type)
            {
                case BRUTER_TYPE_FLOAT:
                    if (b.value.f == 0.0)
                    {
                        fprintf(stderr, "ERROR: Division by zero\n");
                        exit(EXIT_FAILURE);
                    }
                    bruter_push_float(stack, a.value.f / b.value.f, NULL, BRUTER_TYPE_FLOAT);
                    break;
                default:
                    if (b.value.i == 0)
                    {
                        fprintf(stderr, "ERROR: Division by zero\n");
                        exit(EXIT_FAILURE);
                    }
                    bruter_push_float(stack, a.value.f / b.value.i, NULL, BRUTER_TYPE_FLOAT);
                    break;
            }
            break;
        default:
            switch (b.type)
            {
                case BRUTER_TYPE_FLOAT:
                    if (b.value.f == 0.0)
                    {
                        fprintf(stderr, "ERROR: Division by zero\n");
                        exit(EXIT_FAILURE);
                    }
                    bruter_push_float(stack, a.value.i / b.value.f, NULL, BRUTER_TYPE_FLOAT);
                    break;
                default:
                    if (b.value.i == 0)
                    {
                        fprintf(stderr, "ERROR: Division by zero\n");
                        exit(EXIT_FAILURE);
                    }
                    bruter_push_int(stack, a.value.i / b.value.i, NULL, BRUTER_TYPE_ANY);
                    break;
            }
            break;
    }
}

void feraw_mod(BruterList* stack)
{
    BruterMeta a = bruter_pop_meta(stack);
    BruterMeta b = bruter_pop_meta(stack);
    if (a.type == BRUTER_TYPE_FLOAT || b.type == BRUTER_TYPE_FLOAT)
    {
        fprintf(stderr, "ERROR: Modulus operator not supported for float types\n");
        exit(EXIT_FAILURE);
    }
    if (b.value.i == 0)
    {
        fprintf(stderr, "ERROR: Division by zero in modulus operation\n");
        exit(EXIT_FAILURE);
    }
    bruter_push_int(stack, a.value.i % b.value.i, NULL, BRUTER_TYPE_ANY);
}

void feraw_pow(BruterList* stack)
{
    BruterMeta a = bruter_pop_meta(stack);
    BruterMeta b = bruter_pop_meta(stack);
    if (a.type == BRUTER_TYPE_FLOAT || b.type == BRUTER_TYPE_FLOAT)
    {
        bruter_push_float(stack, pow(a.value.f, b.value.f), NULL, BRUTER_TYPE_FLOAT);
    }
    else
    {
        bruter_push_int(stack, (BruterInt)pow((double)a.value.i, (double)b.value.i), NULL, BRUTER_TYPE_ANY);
    }
}

void feraw_sqrt(BruterList* stack)
{
    BruterMeta a = bruter_pop_meta(stack);
    if (a.type != BRUTER_TYPE_FLOAT)
    {
        fprintf(stderr, "ERROR: Square root function requires a float argument\n");
        exit(EXIT_FAILURE);
    }
    if (a.value.f < 0.0)
    {
        fprintf(stderr, "ERROR: Cannot compute square root of negative number\n");
        exit(EXIT_FAILURE);
    }
    bruter_push_float(stack, sqrt(a.value.f), NULL, BRUTER_TYPE_FLOAT);
}

void feraw_abs(BruterList* stack)
{
    BruterMeta a = bruter_pop_meta(stack);
    if (a.type == BRUTER_TYPE_FLOAT)
    {
        bruter_push_float(stack, fabs(a.value.f), NULL, BRUTER_TYPE_FLOAT);
    }
    else
    {
        bruter_push_int(stack, a.value.i < 0 ? -a.value.i : a.value.i, NULL, BRUTER_TYPE_ANY);
    }
}

void feraw_min(BruterList* stack)
{
    BruterMeta a = bruter_pop_meta(stack);
    BruterMeta b = bruter_pop_meta(stack);
    if (a.type == BRUTER_TYPE_FLOAT || b.type == BRUTER_TYPE_FLOAT)
    {
        double val_a = (a.type == BRUTER_TYPE_FLOAT) ? a.value.f : (double)a.value.i;
        double val_b = (b.type == BRUTER_TYPE_FLOAT) ? b.value.f : (double)b.value.i;
        bruter_push_float(stack, fmin(val_a, val_b), NULL, BRUTER_TYPE_FLOAT);
    }
    else
    {
        bruter_push_int(stack, (a.value.i < b.value.i) ? a.value.i : b.value.i, NULL, BRUTER_TYPE_ANY);
    }
}

void feraw_max(BruterList* stack)
{
    BruterMeta a = bruter_pop_meta(stack);
    BruterMeta b = bruter_pop_meta(stack);
    if (a.type == BRUTER_TYPE_FLOAT || b.type == BRUTER_TYPE_FLOAT)
    {
        double val_a = (a.type == BRUTER_TYPE_FLOAT) ? a.value.f : (double)a.value.i;
        double val_b = (b.type == BRUTER_TYPE_FLOAT) ? b.value.f : (double)b.value.i;
        bruter_push_float(stack, fmax(val_a, val_b), NULL, BRUTER_TYPE_FLOAT);
    }
    else
    {
        bruter_push_int(stack, (a.value.i > b.value.i) ? a.value.i : b.value.i, NULL, BRUTER_TYPE_ANY);
    }
}

void feraw_sin(BruterList* stack)
{
    BruterMeta a = bruter_pop_meta(stack);
    if (a.type != BRUTER_TYPE_FLOAT)
    {
        fprintf(stderr, "ERROR: Sine function requires a float argument\n");
        exit(EXIT_FAILURE);
    }
    bruter_push_float(stack, sin(a.value.f), NULL, BRUTER_TYPE_FLOAT);
}

void feraw_cos(BruterList* stack)
{
    BruterMeta a = bruter_pop_meta(stack);
    if (a.type != BRUTER_TYPE_FLOAT)
    {
        fprintf(stderr, "ERROR: Cosine function requires a float argument\n");
        exit(EXIT_FAILURE);
    }
    bruter_push_float(stack, cos(a.value.f), NULL, BRUTER_TYPE_FLOAT);
}

void feraw_tan(BruterList* stack)
{
    BruterMeta a = bruter_pop_meta(stack);
    if (a.type != BRUTER_TYPE_FLOAT)
    {
        fprintf(stderr, "ERROR: Tangent function requires a float argument\n");
        exit(EXIT_FAILURE);
    }
    bruter_push_float(stack, tan(a.value.f), NULL, BRUTER_TYPE_FLOAT);
}

void feraw_inc(BruterList* stack)
{
    BruterMeta a = bruter_pop_meta(stack);
    if (a.type == BRUTER_TYPE_FLOAT)
    {
        a.value.f += 1.0;
        bruter_push_float(stack, a.value.f, NULL, BRUTER_TYPE_FLOAT);
    }
    else
    {
        a.value.i += 1;
        bruter_push_int(stack, a.value.i, NULL, BRUTER_TYPE_ANY);
    }
}

void feraw_dec(BruterList* stack)
{
    BruterMeta a = bruter_pop_meta(stack);
    if (a.type == BRUTER_TYPE_FLOAT)
    {
        a.value.f -= 1.0;
        bruter_push_float(stack, a.value.f, NULL, BRUTER_TYPE_FLOAT);
    }
    else
    {
        a.value.i -= 1;
        bruter_push_int(stack, a.value.i, NULL, BRUTER_TYPE_ANY);
    }
}

// random functions

void feraw_seed(BruterList* stack)
{
    BruterMeta seed_meta = bruter_pop_meta(stack);
    unsigned int seed;
    if (seed_meta.type == BRUTER_TYPE_FLOAT)
    {
        seed = (unsigned int)seed_meta.value.f;
    }
    else
    {
        seed = (unsigned int)seed_meta.value.i;
    }
    srand(seed);
}

void feraw_rand(BruterList* stack)
{
    BruterFloat result = rand();
    bruter_push_float(stack, result, NULL, BRUTER_TYPE_FLOAT);
}

void feraw_random(BruterList* stack)
{
    BruterMeta min_meta = bruter_pop_meta(stack);
    BruterMeta max_meta = bruter_pop_meta(stack);
    if (min_meta.type == BRUTER_TYPE_FLOAT || max_meta.type == BRUTER_TYPE_FLOAT)
    {
        BruterFloat min = 0, max = 0;
        switch (min_meta.type)
        {
            case BRUTER_TYPE_FLOAT:
                min = min_meta.value.f;
                break;
            default:
                min = (BruterFloat)min_meta.value.i;
                break;
        }

        switch (max_meta.type)
        {
            case BRUTER_TYPE_FLOAT:
                max = max_meta.value.f;
                break;
            default:
                max = (BruterFloat)max_meta.value.i;
                break;
        }

        BruterFloat result = fmod(rand(), (max - min + 1) + min);
        bruter_push_float(stack, result, NULL, BRUTER_TYPE_FLOAT);
    }
    else // definively int
    {
        BruterInt min, max;
        min = min_meta.value.i;
        max = min_meta.value.i;
        BruterInt result = rand() % (max - min + 1) + min;
        bruter_push_int(stack, result, NULL, BRUTER_TYPE_ANY);
    }
}

// rouding functions

void feraw_floor(BruterList* stack)
{
    BruterFloat value = bruter_pop_float(stack);
    bruter_push_int(stack, (BruterInt)floor(value), NULL, BRUTER_TYPE_ANY);
}

void feraw_ceil(BruterList* stack)
{
    BruterFloat value = bruter_pop_float(stack);
    bruter_push_int(stack, (BruterInt)ceil(value), NULL, BRUTER_TYPE_ANY);
}

void feraw_round(BruterList* stack)
{
    BruterFloat value = bruter_pop_float(stack);
    bruter_push_int(stack, (BruterInt)round(value), NULL, BRUTER_TYPE_ANY);
}

void feraw_trunc(BruterList* stack)
{
    BruterFloat value = bruter_pop_float(stack);
    bruter_push_int(stack, (BruterInt)trunc(value), NULL, BRUTER_TYPE_ANY);
}

// bitwise operations
void feraw_bit_and(BruterList* stack)
{
    BruterMeta a = bruter_pop_meta(stack);
    BruterMeta b = bruter_pop_meta(stack);
    if (a.type == BRUTER_TYPE_FLOAT || b.type == BRUTER_TYPE_FLOAT)
    {
        fprintf(stderr, "ERROR: Bitwise AND operator not supported for float types\n");
        exit(EXIT_FAILURE);
    }
    bruter_push_int(stack, a.value.i & b.value.i, NULL, BRUTER_TYPE_ANY);
}

void feraw_bit_or(BruterList* stack)
{
    BruterMeta a = bruter_pop_meta(stack);
    BruterMeta b = bruter_pop_meta(stack);
    if (a.type == BRUTER_TYPE_FLOAT || b.type == BRUTER_TYPE_FLOAT)
    {
        fprintf(stderr, "ERROR: Bitwise OR operator not supported for float types\n");
        exit(EXIT_FAILURE);
    }
    bruter_push_int(stack, a.value.i | b.value.i, NULL, BRUTER_TYPE_ANY);
}

void feraw_bit_xor(BruterList* stack)
{
    BruterMeta a = bruter_pop_meta(stack);
    BruterMeta b = bruter_pop_meta(stack);
    if (a.type == BRUTER_TYPE_FLOAT || b.type == BRUTER_TYPE_FLOAT)
    {
        fprintf(stderr, "ERROR: Bitwise XOR operator not supported for float types\n");
        exit(EXIT_FAILURE);
    }
    bruter_push_int(stack, a.value.i ^ b.value.i, NULL, BRUTER_TYPE_ANY);
}

void feraw_bit_not(BruterList* stack)
{
    BruterMeta a = bruter_pop_meta(stack);
    if (a.type == BRUTER_TYPE_FLOAT)
    {
        fprintf(stderr, "ERROR: Bitwise NOT operator not supported for float types\n");
        exit(EXIT_FAILURE);
    }
    bruter_push_int(stack, ~a.value.i, NULL, BRUTER_TYPE_ANY);
}

void feraw_lshift(BruterList* stack)
{
    BruterMeta a = bruter_pop_meta(stack);
    BruterMeta b = bruter_pop_meta(stack);
    if (a.type == BRUTER_TYPE_FLOAT || b.type == BRUTER_TYPE_FLOAT)
    {
        fprintf(stderr, "ERROR: Bitwise shift operators not supported for float types\n");
        exit(EXIT_FAILURE);
    }
    bruter_push_int(stack, a.value.i << b.value.i, NULL, BRUTER_TYPE_ANY);
}

void feraw_rshift(BruterList* stack)
{
    BruterMeta a = bruter_pop_meta(stack);
    BruterMeta b = bruter_pop_meta(stack);
    if (a.type == BRUTER_TYPE_FLOAT || b.type == BRUTER_TYPE_FLOAT)
    {
        fprintf(stderr, "ERROR: Bitwise shift operators not supported for float types\n");
        exit(EXIT_FAILURE);
    }
    bruter_push_int(stack, a.value.i >> b.value.i, NULL, BRUTER_TYPE_ANY);
}

void feraw_neg(BruterList* stack)
{
    BruterMeta a = bruter_pop_meta(stack);
    if (a.type == BRUTER_TYPE_FLOAT)
    {
        bruter_push_float(stack, -a.value.f, NULL, BRUTER_TYPE_FLOAT);
    }
    else
    {
        bruter_push_int(stack, -a.value.i, NULL, BRUTER_TYPE_ANY);
    }
}