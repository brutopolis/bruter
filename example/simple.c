// 100% ai generated code
#include <stdio.h>
#include "bruter.h" // inclui sua definição de API + structs

void print_value(BruterValue v, const char *label) {
    printf("%s:\n", label);
    printf("  inteiro:      %" PRIdPTR "\n", v.i);
    printf("  unsigned:     %" PRIuPTR "\n", v.u);
    printf("  float:        %f\n", v.f);
    printf("  como ponteiro: %p\n", v.p);
    printf("\n");
}

int main(void) {
    // Cria uma lista com capacidade 4 e 1 buffer
    BruterList *list = bruter_init(4, 1);

    // Push de valores diversos
    bruter_push(list, bruter_value_i(10));
    bruter_push(list, bruter_value_u(100));
    bruter_push(list, bruter_value_f(3.14));
    bruter_push(list, bruter_value_p((void *)main));

    // Itera e imprime os valores interpretando como necessário
    for (BruterInt i = 0; i < list->size; ++i) {
        BruterValue v = bruter_get(list, 0, i);
        char label[32];
        snprintf(label, sizeof(label), "Item %" PRIdPTR, i);
        print_value(v, label);
    }

    // Swap dos dois primeiros
    bruter_swap(list, 0, 1);

    puts("Depois de swap(0,1):");
    for (BruterInt i = 0; i < list->size; ++i) {
        BruterValue v = bruter_get(list, 0, i);
        char label[32];
        snprintf(label, sizeof(label), "Item %" PRIdPTR, i);
        print_value(v, label);
    }

    // Inverter a lista
    bruter_reverse(list);

    puts("Depois de reverse:");
    for (BruterInt i = 0; i < list->size; ++i) {
        BruterValue v = bruter_get(list, 0, i);
        char label[32];
        snprintf(label, sizeof(label), "Item %" PRIdPTR, i);
        print_value(v, label);
    }

    // Remove o último
    bruter_pop(list);

    puts("Depois de pop:");
    for (BruterInt i = 0; i < list->size; ++i) {
        BruterValue v = bruter_get(list, 0, i);
        char label[32];
        snprintf(label, sizeof(label), "Item %" PRIdPTR, i);
        print_value(v, label);
    }

    // Libera a memória
    bruter_free(list);
    return 0;
}
