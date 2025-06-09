// 100% ai generated code
#include <stdio.h>
#include "bruter.h" // inclui sua definição de API + structs
#include <ctype.h>

typedef struct SplitedString 
{
    char *str;  // buffer contendo todos os tokens separados por \0
    int count;  // número de tokens
} SplitedString;

static inline SplitedString br_str_space_split(const char *input)
{
    char *str = strdup(input); // buffer mutável
    int i = 0, count = 0;
    bool in_token = false;

    while (str[i] != '\0')
    {
        if (isspace((unsigned char)str[i]))
        {
            str[i] = '\0';
            in_token = false;
            i++;
            continue;
        }

        if (!in_token)
        {
            count++;
            in_token = true;
        }

        // Trata blocos aninhados
        if (str[i] == '(' || str[i] == '{' || str[i] == '[')
        {
            char open = str[i];
            char close = (open == '(') ? ')' : (open == '{' ? '}' : ']');
            int j = i + 1, nested = 1;

            while (str[j] != '\0' && nested > 0)
            {
                if (str[j] == open && str[j - 1] != '\\') nested++;
                else if (str[j] == close && str[j - 1] != '\\') nested--;
                j++;
            }

            // Adiciona \0 após bloco completo
            if (str[j] != '\0') {
                str[j] = '\0';
                i = j + 1;
                in_token = false;
            } else {
                // sem fechamento válido, termina normalmente
                i = j;
            }
        }
        else
        {
            // anda até o próximo espaço ou fim
            int j = i;
            while (str[j] != '\0' && !isspace((unsigned char)str[j])) j++;
            if (str[j] != '\0') {
                str[j] = '\0';
                i = j + 1;
                in_token = false;
            } else {
                i = j;
            }
        }
    }

    return (SplitedString){ .str = str, .count = count };
}


static inline SplitedString br_str_split(const char *input, char delim)
{
    char *str = strdup(input); // modificável
    int i = 0, count = 0;
    int parens = 0, curly = 0, bracket = 0;
    bool in_token = false;

    while (str[i] != '\0')
    {
        char c = str[i];
        if (c == '(' && (i == 0 || str[i - 1] != '\\') && !curly && !bracket) parens++;
        else if (c == ')' && (i == 0 || str[i - 1] != '\\') && !curly && !bracket) parens--;
        else if (c == '{' && (i == 0 || str[i - 1] != '\\') && !parens && !bracket) curly++;
        else if (c == '}' && (i == 0 || str[i - 1] != '\\') && !parens && !bracket) curly--;
        else if (c == '[' && (i == 0 || str[i - 1] != '\\') && !parens && !curly) bracket++;
        else if (c == ']' && (i == 0 || str[i - 1] != '\\') && !parens && !curly) bracket--;

        if (c == delim && !parens && !curly && !bracket)
        {
            str[i] = '\0';
            in_token = false;
            count++;
        }
        else if (!in_token && c != delim)
        {
            in_token = true;
        }

        i++;
    }

    // Contabiliza o último token (caso não termine em delim)
    if (in_token)
        count++;

    return (SplitedString){ .str = str, .count = count };
}



void print_tokens(SplitedString ss) {
    const char *ptr = ss.str;
    for (int i = 0; i < ss.count; i++) {
        printf("Token %d: %s\n", i, ptr);
        ptr += strlen(ptr) + 1; // avança para próximo token
    }
}


int main(void) {
    

    SplitedString result = br_str_space_split("a b (c d) {e f} [g h]");

    print_tokens(result);

    // liberar depois
    free(result.str);


    return 0;
}
