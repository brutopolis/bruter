#ifndef BRUTERC_H
#define BRUTERC_H
#include "bruter.h"

#include <limits.h>
#include <float.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <stddef.h>

#define BRUTERC_VERSION "1.0.8"

typedef struct SplitedString 
{
    char *str;  // buffer with the tokens separated by \0
    int count;  // token count
} SplitedString;

// string stuff
static inline char* br_str_duplicate(const char *str)
{
    BruterInt len = strlen(str);
    char *dup = (char*)malloc(len + 1);
    
    if (dup == NULL)
    {
        printf("BR_ERROR: failed to allocate memory for string duplication\n");
        exit(EXIT_FAILURE);
    }
    
    memcpy(dup, str, len + 1);
    return dup;
}

static inline char* br_str_nduplicate(const char *str, BruterUInt n)
{
    char *dup = (char*)malloc(n + 1);
    
    if (dup == NULL)
    {
        printf("BR_ERROR: failed to allocate memory for string duplication\n");
        exit(EXIT_FAILURE);
    }
    
    memcpy(dup, str, n);
    dup[n] = '\0';
    return dup;
}

static inline char* br_str_format(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    BruterInt size = vsnprintf(NULL, 0, format, args);
    va_end(args);
    char *str = (char*)malloc(size + 1);
    if (str == NULL)
    {
        printf("BR_ERROR: failed to allocate memory for formatted string\n");
        exit(EXIT_FAILURE);
    }
    va_start(args, format);
    vsnprintf(str, size + 1, format, args);
    va_end(args);
    return str;
}

static inline SplitedString br_str_space_split(const char *input)
{
    char *str = strdup(input);
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

            if (str[j] != '\0') 
            {
                str[j] = '\0';
                i = j + 1;
                in_token = false;
            } 
            else 
            {
                i = j;
            }
        }
        else
        {
            int j = i;
            while (str[j] != '\0' && !isspace((unsigned char)str[j])) j++;
            if (str[j] != '\0') 
            {
                str[j] = '\0';
                i = j + 1;
                in_token = false;
            } 
            else 
            {
                i = j;
            }
        }
    }

    return (SplitedString){ .str = str, .count = count };
}


static inline SplitedString br_str_split(const char *input, char delim)
{
    char *str = strdup(input);
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

    if (in_token)
        count++;

    return (SplitedString){ .str = str, .count = count };
}

static inline char* br_get_token(SplitedString splited, BruterInt index)
{
    if (index < 0 || index >= splited.count)
    {
        return NULL; // out of bounds
    }
    else 
    {
        char *token = splited.str;
        for (BruterInt i = 0; i < index; i++)
        {
            token = strchr(token, '\0') + 1; // move to the next token
        }
        return token; // return the token at the specified index
    }
}

#define function(name) BruterInt name(BruterList *list)
#define init(name) void init_##name(BruterList *list)

#define parser_step(name) bool name(BruterList *context, BruterList *result, char* str)
typedef bool (*ParserStep)(BruterList*, BruterList*, char*);

typedef struct VirtualMachine
{
    BruterList *context; // the context of the virtual machine
    BruterList *parser;  // the parser for the commands
    BruterList *unused;  // unused commands
    BruterList *allocs;  // allocated memory for the virtual machine
    BruterList *stack;   // stack for the virtual machine
} VirtualMachine;

static inline BruterList* br_new_context(BruterInt size)
{
    BruterList *new_context = bruter_init(size, 2);
    if (new_context == NULL)
    {
        printf("BR_ERROR: failed to allocate memory for new context\n");
        exit(EXIT_FAILURE);
    }

    bruter_push(new_context, (BruterValue){.p = (void*)new_context}, br_str_duplicate("context"));
    bruter_push(new_context, (BruterValue){.p = bruter_init(8, 1)}, br_str_duplicate("parser")); 
    bruter_push(new_context, (BruterValue){.p = bruter_init(8, 1)}, br_str_duplicate("unused"));
    bruter_push(new_context, (BruterValue){.p = bruter_init(8, 1)}, br_str_duplicate("allocs"));
    bruter_push(new_context, (BruterValue){.p = bruter_init(8, 1)}, br_str_duplicate("stack"));
    return new_context;
}

// Parser functions
static inline BruterList* br_parse(BruterList *context, const char *cmd) 
{
    BruterList *result = bruter_init(sizeof(void*), 1);
    
    SplitedString splited = br_str_space_split(cmd);
    char* tok = splited.str;
    BruterInt current_word = 0;

    while (current_word < splited.count)
    {
        for (BruterInt j = 0; j < parser->size; j++)
        {
            ParserStep step = (ParserStep)parser->data[0][j].p;
            if (step(result, tok))
            {
                // if the step returns true, means it was successful
                // we can break the loop and continue to the next string
                break;
            }
        }

        tok = splited.str + current_word; // 
    };

    free(splited.str);
    return result;
}


static inline parser_step(parser_expression)
{
    if (str[0] == '(')
    {
        char* temp = str + 1;
        temp[strlen(temp) - 1] = '\0';
        BruterInt res = br_eval(context, parser, temp);
        bruter_push(result, (BruterValue){.i = res}, NULL);
        return true;
    }
    return false;
}

static inline BruterValue br_parser_number(const char *str)
{
    BruterValue result;
    if (str[0] == '0' && str[1] == 'x') // hex
    {
        result.i = strtol(str+2, NULL, 16);
    }
    else if (str[0] == '0' && str[1] == 'b') // bin
    {
        result.i = strtol(str+2, NULL, 2);
    }
    else if (str[0] == '0' && str[1] == 'o') // oct
    {
        result.i = strtol(str+2, NULL, 8);
    }
    else if (strchr(str, '.')) // float
    {
        result.f = atof(str);
    }
    else // int
    {
        result.i = atol(str);
    }
    return result;
}

static inline BruterInt br_interpret(BruterList *context, BruterList* parser, const char *cmd)
{
    // [function pointer, context, ...args]
    BruterList *result = br_parse(context, parser, cmd);
    // check if the first element is not null
    if (result->data[0][0].p != NULL)
    {
        BruterValue (*func)(BruterList*) = (BruterValue (*)(BruterList*))result->data[0][0].p; // assuming the first element is a function pointer
        BruterList *args = (BruterList*)result->data[0][1].p; // assuming the second element is the context
        BruterValue value = func(args);
        bruter_free(result);
        return value.i; // return the integer value
    }
    else 
    {
        bruter_free(result);
        return -1; // error
    }
}



static inline BruterInt br_eval(BruterList *context, BruterList* parser, const char *cmd)
{
    BruterList *result = br_parse(context, parser, cmd);
    
    if (result->size > 0 && result->data[0].i != -1)
    {
        BruterInt value = result->data[0].i;
        bruter_free(result);
        return value;
    }
    else 
    {
        bruter_free(result);
        return -1; // error
    }
}

#endif