#include "bruter.h"

// file stuff
char* readfile(char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        return NULL;
    }

    char *code = (char*)malloc(1);
    if (code == NULL)
    {
        printf("BRUTER_ERROR: could not allocate memory for file\n");
        fclose(file);
        return NULL;
    }

    code[0] = '\0';

    char *line = NULL;
    size_t len = 0;
    while (getline(&line, &len, file) != -1)
    {
        size_t new_size = strlen(code) + strlen(line) + 1;
        char *temp = realloc(code, new_size);
        if (temp == NULL)
        {
            printf("BRUTER_ERROR: could not reallocate memory while reading file\n");
            free(code);
            free(line);
            fclose(file);
            return NULL;
        }
        code = temp;
        strcat(code, line);
    }

    free(line);
    fclose(file);
    return code;
}

void writefile(char *filename, char *code)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        return;
    }

    fprintf(file, "%s", code);
    fclose(file);
}

bool file_exists(char* filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        return false;
    }

    fclose(file);
    return true;
}

// string stuff
char* str_duplicate(const char *str)
{
    Int len = strlen(str);
    char *dup = (char*)malloc(len + 1);
    
    if (dup == NULL)
    {
        printf("BRUTER_ERROR: failed to allocate memory for string duplication\n");
        exit(EXIT_FAILURE);
    }
    
    memcpy(dup, str, len + 1);
    return dup;
}

char* str_nduplicate(const char *str, UInt n)
{
    char *dup = (char*)malloc(n + 1);
    
    if (dup == NULL)
    {
        printf("BRUTER_ERROR: failed to allocate memory for string duplication\n");
        exit(EXIT_FAILURE);
    }
    
    memcpy(dup, str, n);
    dup[n] = '\0';
    return dup;
}

List *list_init(Int size)
{
    List *list = (List*)malloc(sizeof(List));
    
    if (list == NULL)
    {
        printf("BRUTER_ERROR: failed to allocate memory for List\n");
        exit(EXIT_FAILURE);
    }
    
    list->data = (size == 0) ? NULL : (Value*)malloc(size * sizeof(Value));
    
    if (size > 0 && list->data == NULL)
    {
        printf("BRUTER_ERROR: failed to allocate memory for List data\n");
        free(list);
        exit(EXIT_FAILURE);
    }
    
    list->size = 0;
    list->capacity = size;
    return list;
}

void list_free(List *list)
{
    free(list->data);
    free(list);
}

void list_double(List *list)
{
    list->capacity = list->capacity == 0 ? 1 : list->capacity * 2;
    list->data = realloc(list->data, list->capacity * sizeof(Value));
}

void list_half(List *list)
{
    list->capacity /= 2;
    list->data = realloc(list->data, list->capacity * sizeof(Value));
    if (list->size > list->capacity)
    {
        list->size = list->capacity;
    }
}

void list_push(List *list, Value value)
{
    if (list->size == list->capacity)
    {
        list_double(list);
    }
    list->data[list->size++] = value;
}

void list_unshift(List *list, Value value)
{
    if (list->size == list->capacity)
    {
        list_double(list);
    }
    memmove(&(list->data[1]), &(list->data[0]), list->size * sizeof(Value));
    list->data[0] = value;
    list->size++;
}

Value list_pop(List *list)
{
    return list->data[--list->size];
}

Value list_shift(List *list)
{
    Value ret = list->data[0];
    if (list->size > 1) 
    {
        memmove(&(list->data[0]), &(list->data[1]), (list->size - 1) * sizeof(Value));
    }
    list->size--;
    return ret;
}


void list_swap(List *list, Int i1, Int i2)
{
    Value tmp = list->data[i1];
    list->data[i1] = list->data[i2];
    list->data[i2] = tmp;
}

void list_insert(List *list, Int i, Value value)
{
    if (list->size == list->capacity)
    {
        list_double(list);
    }
    if (i <= list->size) 
    {
        memmove(&(list->data[i + 1]), &(list->data[i]), (list->size - i) * sizeof(Value));
        list->data[i] = value;
        list->size++;
    } 
    else 
    {
        printf("BRUTER_ERROR: index %" PRIdPTR "  out of range in list of size %" PRIdPTR " \n", i, list->size);
        exit(EXIT_FAILURE);
    }
}

Value list_remove(List *list, Int i)
{
    Value ret = list->data[i];
    memmove(&(list->data[i]), &(list->data[i + 1]), (list->size - (i) - 1) * sizeof(Value));
    list->size--;
    return ret;
}

Value list_fast_remove(List *list, Int i)
{
    Value ret = list->data[i];
    list_swap(list, i, list->size - 1);
    list_pop(list);
    return ret;
}

Int list_ocurrences(List *list, Value value)
{
    Int i = 0;
    while (i < list->size && list->data[i].i != value.i)
    {
        i++;
    }
    return i == list->size ? -1 : i;
}

Int list_find(List *list, Value value)
{
    Int i = -1;
    for (Int j = 0; j < list->size; j++)
    {
        if (list->data[j].i == value.i)
        {
            i = j;
            break;
        }
    }
    return i;
}

void list_reverse(List *list)
{
    for (Int i = 0; i < list->size / 2; i++)
    {
        list_swap(list, i, list->size - i - 1);
    }
}

char* str_format(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    Int size = vsnprintf(NULL, 0, format, args);
    va_end(args);
    char *str = (char*)malloc(size + 1);
    if (str == NULL)
    {
        printf("BRUTER_ERROR: failed to allocate memory for formatted string\n");
        exit(EXIT_FAILURE);
    }
    va_start(args, format);
    vsnprintf(str, size + 1, format, args);
    va_end(args);
    return str;
}

List* special_space_split(char *str)
{
    List *splited = list_init(sizeof(void*));
    
    int i = 0;
    while (str[i] != '\0')
    {
        if (str[i] == '(')
        {
            int j = i;
            int count = 1;
            while (count != 0)
            {
                j++;
                if (str[j] == '(' && (j == 0 || str[j-1] != '\\'))  // não conta se for escapado
                {
                    count++;
                }
                else if (str[j] == ')' && (j == 0 || str[j-1] != '\\'))  // não conta se for escapado
                {
                    count--;
                }
            }
            char *tmp = str_nduplicate(str + i, j - i + 1);
            list_push(splited, (Value){.s = tmp});
            i = j + 1;
        }
        else if (str[i] == '{')
        {
            int j = i;
            int count = 1;
            while (count != 0)
            {
                j++;
                if (str[j] == '{' && (j == 0 || str[j-1] != '\\'))  // não conta se for escapado
                {
                    count++;
                }
                else if (str[j] == '}' && (j == 0 || str[j-1] != '\\'))  // não conta se for escapado
                {
                    count--;
                }
            }
            char *tmp = str_nduplicate(str + i, j - i + 1);
            list_push(splited, (Value){.s = tmp});
            i = j + 1;
        }
        else if (isspace(str[i]))
        {
            i++;
        }
        else
        {
            int j = i;
            while (!isspace(str[j]) && str[j] != '\0' && str[j] != '(' && str[j] != ')')
            {
                j++;
            }
            list_push(splited, (Value) {.s = str_nduplicate(str + i, j - i)});
            i = j;
        }
    }
    return splited;
}


List* special_split(char *str, char delim)
{
    List *splited = list_init(sizeof(void*));
    
    int recursion = 0;
    int curly = 0;
    
    int i = 0;
    int last_i = 0;

    while (str[i] != '\0')
    {
        if (str[i] == '(' && (i == 0 || str[i-1] != '\\') && !curly)
        {
            recursion++;
        }
        else if (str[i] == ')' && (i == 0 || str[i-1] != '\\') && !curly)
        {
            recursion--;
        }
        else if (str[i] == '{' && (i == 0 || str[i-1] != '\\') && !recursion)
        {
            curly++;
        }
        else if (str[i] == '}' && (i == 0 || str[i-1] != '\\') && !recursion)
        {
            curly--;
        }

        if (str[i] == delim && !recursion && !curly)
        {
            char* tmp = str_nduplicate(str + last_i, i - last_i);
            list_push(splited, (Value){.s = tmp});
            last_i = i + 1;
        }
        else if (str[i + 1] == '\0')
        {
            char* tmp = str_nduplicate(str + last_i, i - last_i + 1);
            list_push(splited, (Value){.s = tmp});
        }

        i++;
    }
    return splited;
}

// var new 
Int new_var(List *vm)
{
    Value value;
    value.p = NULL;
    list_push(vm, value);
    return vm->size-1;
}

Int new_block(List *vm, Int size)
{
    if (!size)
    {
        return -1;
    }

    list_push(vm, (Value){.i = 0});

    Int index = vm->size - 1;
    
    for (Int i = 0; i < size-1; i++)
    {
        list_push(vm, (Value){.i = 0});
    }
    
    return index;
}

Int new_string(List *vm, char* str)
{
    Int len = strlen(str);
    Int blocks = (len + 1 + sizeof(void*) - 1) / sizeof(void*);
    Int var = new_block(vm, blocks);
    memcpy(&vm->data[var].u8[0], str, len);
    vm->data[var].u8[len] = '\0';
    return var;
}

Value parse_number(char *str)
{
    Value result;
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

// Parser functions
List* parse(void *_vm, char *cmd) 
{
    List* vm = (List*)_vm;
    List *result = list_init(sizeof(void*));
    
    List *splited = special_space_split(cmd);
    char* str = NULL;
    Int i = 0;
    for (i = 0; i < splited->size; i++)
    {
        str = splited->data[i].s;
        
        if (str[0] == '(')
        {
            char* temp = str + 1;
            temp[strlen(temp) - 1] = '\0';
            Int res = eval(vm, temp);
            list_push(result, (Value){.i = res});
        }
        else if (str[0] == '{') // string
        {
            Int len = strlen(str);
            str[len - 1] = '\0';

            list_push(result, (Value){.i = new_string(vm, str + 1)});
        }
        else if ((str[0] >= '0' && str[0] <= '9') || str[0] == '-') // number
        {
            Int index = new_var(vm);
            vm->data[index] = parse_number(str);
            list_push(result, (Value){.i = index});
        }
        else
        {
            // just ignore the string
        }

        free(str);
    }

    list_free(splited);
    return result;
}

Int interpret(List *vm, char* cmd, List* _args)
{
    List *args;

    if (_args != NULL)
    {
        args = _args;
    }
    else 
    {
        args = parse(vm, cmd);
    }
    
    if (!args->size)
    {
        list_free(args);
        return -1;
    }
    
    Int func = list_shift(args).i;
    Int result = -1;
    if (func > -1)
    {
        Function _function;
        _function = vm->data[func].p;
        result = _function(vm, args);
        list_unshift(args, (Value){.p = _function});
    }
    else if (func) // if func is not 0
    {
        printf("BRUTER_ERROR: %" PRIdPTR " is not a function or script\n", func);
    }
    
    list_free(args);
    
    return result;
}

Int eval(List *vm, char *cmd)
{
    if(!strchr(cmd, ';')) // if == NULL
    {
        return interpret(vm, cmd, NULL);
    }

    List *splited = special_split(cmd, ';');

    // remove empty or whitespace-only strings using isspace
    Int last = splited->size - 1;
    for (; last >= 0; last--)
    {
        if (strlen(splited->data[last].s) == 0)
        {
            free(list_pop(splited).p);
        }
        else
        {
            Int i = 0;
            while (splited->data[last].s[i] != '\0' && isspace(splited->data[last].s[i]))
            {
                i++;
            }

            if (splited->data[last].s[i] == '\0')
            {
                free(splited->data[last].s);
                list_pop(splited);
            }
        }
        last--;
    }

    Int result = -1;
    char* str = NULL;
    for (Int i = 0; i < splited->size; i++)
    {        
        str = splited->data[i].s;
        result = interpret(vm, str, NULL);
        free(str);
        if (result > 0)
        {
            for (Int j = i + 1; j < splited->size; j++)
            {
                free(splited->data[j].s);
            }
            break;
        }
    }
    list_free(splited);
    return result;
}