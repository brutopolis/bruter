#include "bruter.h"

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
    List *splited = list_init(0);
    
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
                if (str[j] == '(')
                {
                    count++;
                }
                else if (str[j] == ')')
                {
                    count--;
                }
            }
            char *tmp = str_nduplicate(str + i, j - i + 1);
            list_push(splited, (Value){.s = tmp});
            i = j + 1;
        }
        else if (str[i] == '"')
        {
            int j = i;
            j++;
            while (str[j] != '"' && str[j] != '\0')
            {
                j++;
            }
            char *tmp = str_nduplicate(str + i, j - i + 1);
            list_push(splited, (Value){.s = tmp});
            i = j + 1;
        }
        else if (str[i] == '\'')
        {
            int j = i;
            j++;
            while (str[j] != '\'' && str[j] != '\0')
            {
                j++;
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
    List *splited = list_init(2);
    
    int recursion = 0;
    bool inside_double_quotes = 0;
    bool inside_single_quotes = 0;
    int i = 0;
    int last_i = 0;

    while (str[i] != '\0')
    {
        if (str[i] == '(' && !inside_double_quotes && !inside_single_quotes)
        {
            recursion++;
        }
        else if (str[i] == ')' && !inside_double_quotes && !inside_single_quotes)
        {
            recursion--;
        }
        else if (str[i] == '"' && !recursion && !inside_single_quotes)
        {
            inside_double_quotes = !inside_double_quotes;
        }
        else if (str[i] == '\'' && !recursion && !inside_double_quotes)
        {
            inside_single_quotes = !inside_single_quotes;
        }

        if (str[i] == delim && !recursion && !inside_double_quotes && !inside_single_quotes)
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

//hash functions

Int hash_find(VirtualMachine *vm, char *varname)
{
    for (Int i = 0; i < vm->hashes->size; i++)
    {
        if (vm->hashes->data[i].s != NULL && strcmp(vm->hashes->data[i].s, varname) == 0)
        {
            return i;
        }
    }
    return -1;
}

void hash_set(VirtualMachine *vm, char* varname, Int index)
{
    Int found = hash_find(vm, varname);
    if (index > vm->values->size || index < 0)
    {
        printf("BRUTER_ERROR: index %" PRIdPTR "  out of range in hash of size %" PRIdPTR" \n", index, vm->values->size);
        exit(EXIT_FAILURE);
    }

    if (found != -1)
    {
        vm->hashes->data[index].s = vm->hashes->data[found].s;
        vm->hashes->data[found].s = NULL;
    }
    else 
    {

        vm->hashes->data[index].s = str_duplicate(varname);
    }
}

void hash_unset(VirtualMachine *vm, char* varname)
{
    Int index = hash_find(vm, varname);
    if (index != -1)
    {
        free(vm->hashes->data[index].s);
        vm->hashes->data[index].s = NULL;
    }
}

//variable functions

VirtualMachine* make_vm(Int size)
{
    VirtualMachine *vm = (VirtualMachine*)malloc(sizeof(VirtualMachine));
    if (vm == NULL)
    {
        printf("BRUTER_ERROR: failed to allocate memory for VirtualMachine\n");
        exit(EXIT_FAILURE);
    }
    vm->values = list_init(size);
    vm->hashes = list_init(size);

    // @0 = null
    new_var(vm, "NULL");

    return vm;
}

// var new 
Int new_var(VirtualMachine *vm, char* varname)
{
    char* namestr = (varname == NULL) ? NULL : str_duplicate(varname);

    Value value;
    value.p = NULL;
    list_push(vm->values, value);
    list_push(vm->hashes, (Value){.s = namestr});
    return vm->values->size-1;
}

Int new_block(VirtualMachine *vm, char* varname, Int size)// allocate size slots and 
{
    char* namestr = (varname == NULL) ? NULL : str_duplicate(varname);

    list_push(vm->values, (Value){.i = 0});
    list_push(vm->hashes, (Value){.s = namestr});

    Int index = vm->values->size - 1;
    
    for (Int i = 0; i < size-1; i++)
    {
        list_push(vm->values, (Value){.i = 0});
        list_push(vm->hashes, (Value){.s = NULL});
    }
    
    return index;
}

//frees
void free_vm(VirtualMachine *vm)
{
    for (Int i = 0; i < vm->values->size; i++)
    {
        if (vm->hashes->data[i].s != NULL)
        {
            free(vm->hashes->data[i].s);
        }
    }

    list_free(vm->values);
    list_free(vm->hashes);

    free(vm);
}

// Parser functions
List* parse(void *_vm, char *cmd) 
{
    VirtualMachine* vm = (VirtualMachine*)_vm;
    List *result = list_init(0);
    
    List *splited = special_space_split(cmd);
    char* str = NULL;
    Int i = 0;
    for (i = 0; i < splited->size; i++)
    {
        str = splited->data[i].s;
        
        if (str[0] == '(')
        {
            if(str[1] == '@') //string
            {
                Int len = strlen(str);
                Int blocks = (len+1) / sizeof(void*);
                Int var = new_block(vm, NULL, blocks);
                memcpy(&vm->values->data[var].u8[0], str + 2, len - 3);
                ((uint8_t*)vm->values->data)[(var*sizeof(void*)) + len - 3] = '\0';

                list_push(result, (Value){.i = var});
                
                //continue;
            }
            else
            {
                char* temp = str + 1;
                temp[strlen(temp) - 1] = '\0';
                list_push(result, (Value){.i = eval(vm, temp)});
            }
        }
        else if (str[0] == '"' || str[0] == '\'') // string
        {
            Int len = strlen(str);
            Int blocks = (len+1) / sizeof(void*);
            Int var = new_block(vm, NULL, blocks);
            
            memcpy(&vm->values->data[var].u8[0], str + 1, len - 2);
            ((uint8_t*)vm->values->data)[(var*sizeof(void*)) + len - 2] = '\0';

            list_push(result, (Value){.i = var});

            //continue;
            //free(temp);
        }
        else if (isdigit(str[0]) || str[0] == '-') // number
        {
            if (str[0] == '0' && str[1] == 'x') // hex
            {
                Int i = strtol(str+2, NULL, 16);
                Int var = new_var(vm, NULL);
                data(var).i = i;
                list_push(result, (Value){.i = var});
            }
            else if (str[0] == '0' && str[1] == 'b') // bin
            {
                Int i = strtol(str+2, NULL, 2);
                Int var = new_var(vm, NULL);
                data(var).i = i;
                list_push(result, (Value){.i = var});
            }
            else if (str[0] == '0' && str[1] == 'o') // oct
            {
                Int i = strtol(str+2, NULL, 8);
                Int var = new_var(vm, NULL);
                data(var).i = i;
                list_push(result, (Value){.i = var});
            }
            else if (strchr(str, '.')) // float
            {
                Float f = atof(str);
                Int var = new_var(vm, NULL);
                data(var).f = f;
                list_push(result, (Value){.i = var});
            }
            else // int
            {
                Int i = atol(str);
                Int var = new_var(vm, NULL);
                data(var).i = i;
                list_push(result, (Value){.i = var});
            }
        }
        else //variable 
        {
            Int hashindex = -1;
            hashindex = hash_find(vm, str);

            if (hashindex == -1) 
            {
                printf("BRUTER_ERROR: variable %s not found\n", str);
                list_push(result, (Value){.i = -1});
            }
            else 
            {
                list_push(result, (Value){.i = hashindex});
            }
        }

        free(str);
    }
    list_free(splited);
    return result;
}

Int interpret(VirtualMachine *vm, char* cmd)
{
    List *args = parse(vm, cmd);

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
        _function = vm->values->data[func].p;
        result = _function(vm, args);
        list_unshift(args, (Value){.p = _function});
    }
    else 
    {
        printf("BRUTER_ERROR: %" PRIdPTR " is not a function or script\n", func);
    }
    
    list_free(args);
    
    return result;
}

Int eval(VirtualMachine *vm, char *cmd)
{
    if(!strchr(cmd, ';')) // if == NULL
    {
        return interpret(vm, cmd);
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
        result = interpret(vm, str);
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