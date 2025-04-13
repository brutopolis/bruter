#include "bruter.h"

//string functions
#ifdef _WIN32
char* strndup(const char *str, UInt n)
{
    char *dup = (char*)malloc(n + 1);
    for (Int i = 0; i < n; i++)
    {
        dup[i] = str[i];
    }
    dup[n] = '\0';
    return dup;
}
#endif

char* str_format(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    Int size = vsnprintf(NULL, 0, format, args);
    va_end(args);
    char *str = (char*)malloc(size + 1);
    va_start(args, format);
    vsprintf(str, format, args);
    va_end(args);
    return str;
}

StringList* special_space_split(char *str)
{
    StringList *splited = list_init(StringList);
    
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
            char *tmp = strndup(str + i, j - i + 1);
            list_push(*splited, tmp);
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
            char *tmp = strndup(str + i, j - i + 1);
            list_push(*splited, tmp);
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
            char *tmp = strndup(str + i, j - i + 1);
            list_push(*splited, tmp);
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
            list_push(*splited, strndup(str + i, j - i));
            i = j;
        }
    }
    return splited;
}


StringList* special_split(char *str, char delim)
{
    StringList *splited = list_init(StringList);
    
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
            char* tmp = strndup(str + last_i, i - last_i);
            list_push(*splited, tmp);
            last_i = i + 1;
        }
        else if (str[i + 1] == '\0')
        {
            char* tmp = strndup(str + last_i, i - last_i + 1);
            list_push(*splited, tmp);
        }

        i++;
    }
    return splited;
}

//hash functions

Int hash_find(VirtualMachine *vm, char *varname)
{
    for (Int i = 0; i < vm->hash_names->size; i++)
    {
        if (!strcmp(vm->hash_names->data[i], varname))
        {
            return i;
        }
    }
    return -1;
}

void hash_set(VirtualMachine *vm, char* varname, Int index)
{
    Int found = hash_find(vm, varname);
    if (found != -1)
    {
        vm->hash_indexes->data[found] = index;
    }
    else 
    {
        list_push(*vm->hash_names, strdup(varname));
        list_push(*vm->hash_indexes, index);
    }
}

void hash_unset(VirtualMachine *vm, char* varname)
{
    Int index = hash_find(vm, varname);
    if (index != -1)
    {
        free(list_fast_remove(*vm->hash_names, index));
        list_fast_remove(*vm->hash_indexes, index);
    }
}

//variable functions

VirtualMachine* make_vm()
{
    VirtualMachine *vm = (VirtualMachine*)malloc(sizeof(VirtualMachine));
    vm->stack = list_init(ValueList);
    vm->typestack = list_init(TypeList);
    vm->hash_names = list_init(StringList);
    vm->hash_indexes = list_init(IntList);

    // @0 = null
    register_var(vm, "null", TYPE_DATA, 0);

    return vm;
}

// var new 
Int new_var(VirtualMachine *vm, Type type, Int content)
{
    Value value;
    value.i = content;
    list_push(*vm->stack, value);
    list_push(*vm->typestack, type);
    return vm->stack->size-1;
}

Int register_var(VirtualMachine *vm, char* varname, Type type, Int content)
{
    Int index = new_var(vm, type, content);
    hash_set(vm, varname, index);
    return index;
}

//frees
void free_vm(VirtualMachine *vm)
{
    Value value;
    while (vm->stack->size > 0)
    {
        value = list_pop(*vm->stack);
        switch (list_pop(*vm->typestack).alloc)
        {
            case 1:
                free(value.p);
                break;
        }
    }

    while (vm->hash_names->size > 0)
    {
        free(list_pop(*vm->hash_names));
    }

    list_free(*vm->stack);
    list_free(*vm->typestack);

    list_free(*vm->hash_names);
    list_free(*vm->hash_indexes);

    free(vm);
}

// Parser functions
IntList* parse(void *_vm, char *cmd) 
{
    VirtualMachine* vm = (VirtualMachine*)_vm;
    IntList *result = list_init(IntList);
    
    StringList *splited = special_space_split(cmd);
    list_reverse(*splited);

    //Int current = 0;
    while (splited->size > 0)
    {
        char* str = list_pop(*splited);
        
        if (str[0] == '(')
        {
            if(str[1] == '@' && str[2] == '@') //string
            {
                Int var = new_var(vm, TYPE_STRING, pun(strndup(str+3, strlen(str)-4), s, i));
                list_push(*result, var);
            }
            else
            {
                char* temp = str + 1;
                temp[strlen(temp) - 1] = '\0';
                Int index = eval(vm, temp);
                list_push(*result, index);
            }
        }
        else if (str[0] == '@')
        {
            if (strchr(str, '.')) // float
            {
                list_push(*result, pun((Float)atof(str+1), f, i));
            }
            else // int
            {
                list_push(*result, (Int)atol(str+1));
            }
        }
        else if (str[0] == '"' || str[0] == '\'') // string
        {
            char* temp = strndup(str + 1, strlen(str) - 2);
            Int var = new_var(vm, TYPE_STRING, pun(temp, s, i));
            list_push(*result, var);
            //free(temp);
        }
        else if (isdigit(str[0]) || str[0] == '-') // number
        {
            if (strchr(str, '.')) // float
            {
                Float f = atof(str);
                Int var = new_var(vm, TYPE_FLOAT, pun(f, f, i));
                list_push(*result, var);
            }
            else // int
            {
                Int i = atol(str);
                Int var = new_var(vm, TYPE_DATA, i);
                list_push(*result, var);
            }
        }
        else //variable 
        {
            int hashindex = -1;
            hashindex = hash_find(vm, str);

            if (hashindex == -1) 
            {
                printf("BRUTER_ERROR: variable %s not found\n", str);
                list_push(*result, -1);
            }
            else 
            {
                list_push(*result, vm->hash_indexes->data[hashindex]);
            }
        }

        free(str);
    }
    list_free(*splited);
    return result;
}

Int interpret(VirtualMachine *vm, char* cmd)
{
    IntList *args = parse(vm, cmd);

    if (!args->size)
    {
        list_free(*args);
        return -1;
    }
    
    Int func = list_shift(*args);
    Int result = -1;
    if (func > -1)
    {
        Function _function;

        switch (vm->typestack->data[func].exec)
        {
            case 1:
                switch (vm->typestack->data[func].string)
                {
                    case 1:
                        switch (vm->typestack->data[func].alloc)
                        {
                            case 1:
                                result = eval(vm, vm->stack->data[func].s);
                                list_unshift(*args, func);
                                break;

                            case 0:
                                result = eval(vm, vm->stack->data[func].i8);
                                list_unshift(*args, func);
                                break;
                        }
                        break;

                    case 0:
                        _function = vm->stack->data[func].p;
                        result = _function(vm, args);
                        list_unshift(*args, func);
                        break;
                }
                break;

                
            default:
                printf("BRUTER_ERROR: this value is not executable\n");
                result = -1;
                break;
        }
    }
    else 
    {
        printf("BRUTER_ERROR: %ld is not a function or script\n", func);
    }
    
    list_free(*args);
    
    return result;
}

Int eval(VirtualMachine *vm, char *cmd)
{
    if(!strchr(cmd, ';')) // if == NULL
    {
        return interpret(vm, cmd);
    }

    StringList *splited = special_split(cmd, ';');

    // remove empty or whitespace only strings using isspace
    Int last = splited->size - 1;
    while (last >= 0)
    {
        if (!strlen(splited->data[last]))
        {
            free(list_pop(*splited));
        }
        else
        {
            Int i = 0;
            while (splited->data[last][i] != '\0' && isspace(splited->data[last][i]))
            {
                i++;
            }

            if (splited->data[last][i] == '\0')
            {
                free(splited->data[last]);
                list_pop(*splited);
            }
        }
        last--;
    }

    list_reverse(*splited);
    Int result = -1;
    while (splited->size > 0)
    {
        
        char *str = list_pop(*splited);
        if (!strlen(str))
        {
            free(str);
            continue;
        }
        result = interpret(vm, str);
        free(str);
        if (result > 0)
        {
            while (splited->size > 0)
            {
                free(list_pop(*splited));
            }
            break;
        }
    }
    list_free(*splited);
    return result;
}