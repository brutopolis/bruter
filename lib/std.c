#include "bruter.h"

#ifndef ARDUINO

function(brl_os_file_read)
{
    char *code = readfile(arg(0).string);
    Int index = -1;
    if (code != NULL)
    {
        index = new_var(vm);
        data(index).string = code;
        data_t(index) = TYPE_STRING;
    }
    return index;
}

function(brl_os_file_write)
{
    writefile(arg(0).string, arg(1).string);
    return -1;
}

function(brl_os_file_delete)
{
    remove(arg(0).string);
    return -1;
}

function(brl_os_repl)
{
    return repl(vm);
}

function(brl_os_dofile)
{
    char *code = readfile(arg(0).string);
    Int result = -1;
    if (code != NULL)
    {
        result = eval(vm, code);
        free(code);
    }
    else 
    {
        printf("Error: file %s not found\n", arg(0).string);
    }
    return result;
}

function(brl_os_file_exists)
{
    FILE *file = fopen(arg(0).string, "r");
    Int result = new_number(vm, file != NULL);
    if (file != NULL)
    {
        fclose(file);
    }
    return result;
}

function(brl_os_file_rename)
{
    rename(arg(0).string, arg(1).string);
    return -1;
}

function(brl_os_file_copy)
{
    FILE *file = fopen(arg(0).string, "r");
    if (file == NULL)
    {
        return -1;
    }
    fclose(file);
    char *code = readfile(arg(0).string);
    writefile(arg(1).string, code);
    free(code);
    return -1;
}

function(brl_os_file_size)
{
    FILE *file = fopen(arg(0).string, "r");
    if (file == NULL)
    {
        return -1;
    }
    fseek(file, 0, SEEK_END);
    Int result = new_number(vm, ftell(file));
    fclose(file);
    return result;
}

function(brl_os_time_now)
{
    return new_number(vm, time(NULL));
}

function(brl_os_time_clock)
{
    return new_number(vm, clock());
}

#endif

function(brl_std_hash_set)
{
    hash_set(vm, arg(0).string, arg_i(1));
    hold_var(vm, arg_i(0));
    return -1;
}

function(brl_std_hash_get)
{
    Int index = hash_find(vm, arg(0).string);
    if (index >= 0)
    {
        return hash(index).index;
    }
    return -1;
}

function(brl_std_hash_delete)
{
    hash_unset(vm, arg(0).string);
    return -1;
}

function(brl_std_hash_rename)
{
    Int index = hash_find(vm, arg(0).string);
    if (index >= 0)
    {
        free(hash(index).key);
        hash(index).key = strdup(arg(1).string);
    }
    return -1;
}

function(brl_std_io_print)
{
    for (Int i = 0; i < args->size; i++)
    {
        if (arg_i(i) >= 0 || arg_i(i) < vm->stack->size)
        {
            print_element(vm, arg_i(i));
        }
        else 
        {
            printf("{out of stack}");
        }

        if (args->size > 0)
        {
            printf(" ");
        }
    }
    printf("\n");
    return -1;
}

function(brl_std_io_ls)
{
    Int _var = -1;
    if (args->size > 0)
    {
        _var = arg_i(0);
    }

    if (_var > -1 && data_t(_var) == TYPE_LIST)
    {
        IntList *list = (IntList*)vm->stack->data[(Int)data(_var).number].pointer;
        for (Int i = 0; i < list->size; i++)
        {
            printf("%d[%d]: ", (Int)data(_var).number, i);
            print_element(vm, list->data[i]);
            printf("\n");
        }
    }
    else 
    {
        for (Int i = 0; i < vm->stack->size; i++)
        {
            printf("[%ld]: ", i);
            print_element(vm, i);
            printf("\n");
        }
    }

    return -1;
}

function(brl_std_io_ls_types)
{
    for (Int i = 0; i < vm->stack->size; i++)
    {
        printf("[%ld]: %d\n", i, data_t(i));
    }
    return -1;
}

function(brl_std_io_ls_hashes)
{
    for (Int i = 0; i < vm->hashes->size; i++)
    {
        printf("[%s] {%d} @%ld: ", hash(i).key, vm->typestack->data[hash(i).index], hash(i).index);
        print_element(vm, hash(i).index);
        printf("\n");
    }
    return -1;
}

function(brl_std_io_ls_temp)
{
    for (Int i = 0; i < vm->temp->size; i++)
    {
        printf("[%ld] {temp}: ", vm->temp->data[i]);
        print_element(vm, vm->temp->data[i]);
        printf("\n");
    }
    return -1;
}

function(brl_std_io_ls_unused)
{
    for (Int i = 0; i < vm->unused->size; i++)
    {
        printf("[%ld] {unused}: ", vm->unused->data[i]);
        print_element(vm, vm->unused->data[i]);
        printf("\n");
    }
    return -1;
}

function(brl_std_do)
{
    char* _str = arg(0).string;
    
    Int last_local = hash_find(vm, "local");
    Int local_index = new_list(vm);
    hold_var(vm, local_index);

    if (last_local == -1)
    {
        hash_set(vm, "local", local_index);
    }
    else 
    {
        for (Int i = 0; i < vm->hashes->size; i++)
        {
            if (strcmp(hash(i).key, "local") == 0)
            {
                hash(i).index = local_index;
                break;
            }
        }
    }
    //hash_set(vm, "local", local_index);
    

    for (Int i = 0; i < args->size; i++)
    {
        stack_push(*((IntList*)data(local_index).pointer), args->data[i]);
    }
    Int result = eval(vm, _str);

    if (last_local == -1)
    {
        hash_unset(vm, "local");
    }
    else 
    {
        hash_set(vm, "local", last_local);
    }
    return result;
}


function(brl_std_ignore)
{
    return -1;
}

function(brl_std_return)
{
    return arg_i(0);
}

function(brl_std_gindex)
{
    return new_number(vm, arg_i(0));
}

function(brl_std_type_get)
{
    Int result = new_number(vm, arg_t(0));
    return result;
}

function(brl_std_type_set)
{
    arg_t(0) = (Int)arg(0).number;
    return -1;
}


// math functions
// math functions
// math functions
// math functions


function(brl_std_math_add)
{
    Int result = arg_i(0);
    for (Int i = 1; i < args->size; i++)
    {
        arg(result).number += arg(i).number;
    }
    return -1;
}

function(brl_std_math_sub)
{
    Int result = arg_i(0);
    for (Int i = 1; i < args->size; i++)
    {
        arg(result).number -= arg(i).number;
    }
    return -1;
}

function(brl_std_math_mul)
{
    Int result = arg_i(0);
    for (Int i = 1; i < args->size; i++)
    {
        arg(result).number *= arg(i).number;
    }
    return -1;
}

function(brl_std_math_div)
{
    Int result = arg_i(0);
    for (Int i = 1; i < args->size; i++)
    {
        arg(result).number /= arg(i).number;
    }
    return -1;
}

function(brl_std_math_mod)
{
    arg(0).number = fmod(arg(0).number, arg(1).number);
    return -1;
}

function(brl_std_math_pow)
{
    arg(0).number = pow(arg(0).number, arg(1).number);
    return -1;
}

function(brl_std_math_sqrt)
{
    for (Int i = 0; i < args->size; i++)
    {
        arg(i).number = sqrt(arg(i).number);
    }
    return -1;
}

function(brl_std_math_abs)
{
    for (Int i = 0; i < args->size; i++)
    {
        arg(i).number = fabs(arg(i).number);
    }
    return -1;
}

function(brl_std_math_random)
{
    for (Int i = 0; i < args->size; i++)
    {
        arg(i).number = (Float)rand() / RAND_MAX;
    }
    return -1;
}

function(brl_std_math_seed)
{
    srand(arg(0).number);
    return -1;
}

function(brl_std_math_floor)
{
    for (Int i = 0; i < args->size; i++)
    {
        arg(i).number = floor(arg(i).number);
    }
    return -1;
}

function(brl_std_math_ceil)
{
    for (Int i = 0; i < args->size; i++)
    {
        arg(i).number = ceil(arg(i).number);
    }
    return -1;
}

function(brl_std_math_round)
{
    for (Int i = 0; i < args->size; i++)
    {
        arg(i).number = round(arg(i).number);
    }
    return -1;
}

function(brl_std_math_increment)
{
    for (Int i = 0; i < args->size; i++)
    {
        arg(i).number++;
    }
    return -1;
}

function(brl_std_math_decrement)
{
    for (Int i = 0; i < args->size; i++)
    {
        arg(i).number--;
    }
    return -1;
}


function(int_from_float)
{
    for (Int i = 0; i < args->size; i++)
    {
        arg(i).integer = (Int)arg(i).number;
        arg_t(i) = TYPE_INTEGER;
    }
    return -1;
}

function(int_to_float)
{
    for (Int i = 0; i < args->size; i++)
    {
        arg(i).number = (Float)arg(i).integer;
        arg_t(i) = TYPE_NUMBER;
    }
    return -1;
}

// list functions
// list functions
// list functions
// list functions

function(brl_std_list_new)
{
    Int index = new_list(vm);
    IntList *list = (IntList*)data(index).pointer;
    return index;
}

function(brl_std_list_insert)
{
    if (arg_t(0) == TYPE_LIST)
    {
        IntList *list = (IntList*)arg(0).pointer;
        Int index = arg_i(1);
        Int value = arg_i(2);
        stack_insert(*list, index, value);
    }
    return -1;
}
function(brl_std_list_push)
{
    if (arg_t(0) == TYPE_LIST)
    {
        IntList *list = (IntList*)arg(0).pointer;
        Int value = arg_i(1);
        stack_push(*list, value);
    }
    return -1;
}

function(brl_std_list_unshift)
{
    if (arg_t(0) == TYPE_LIST)
    {
        IntList *list = (IntList*)arg(0).pointer;
        Int value = arg_i(1);
        stack_unshift(*list, value);
    }
    return -1;   
}

function(brl_std_list_remove)// returns the removed element
{
    if (arg_t(0) == TYPE_LIST)
    {
        IntList *list = (IntList*)arg(0).pointer;
        Int index = arg_i(1);
        return stack_remove(*list, index);
    }
    return -1;
}

function(brl_std_list_pop)// returns the removed element
{
    if (arg_t(0) == TYPE_LIST)
    {
        IntList *list = (IntList*)arg(0).pointer;
        return stack_pop(*list);
    }
    return -1;
}

function(brl_std_list_shift)// returns the removed element
{
    if (arg_t(0) == TYPE_LIST)
    {
        IntList *list = (IntList*)arg(0).pointer;
        return stack_shift(*list);
    }
    return -1;
}

function(brl_std_list_concat)
{
    Int _newlist = new_list(vm);
    IntList *newlist = (IntList*)data(_newlist).pointer;
    for (Int i = 0; i < args->size; i++)
    {
        if (arg_t(i) == TYPE_LIST)
        {
            IntList *list = (IntList*)arg(i).pointer;
            for (Int j = 0; j < list->size; j++)
            {
                stack_push(*newlist, list->data[j]);
            }
        }
    }
    return _newlist;
}

function(brl_std_list_find)
{
    Int list = arg_i(0);
    Int value = arg_i(1);
    if (arg_t(0) == TYPE_LIST)
    {
        IntList *lst = (IntList*)data(list).pointer;
        for (Int i = 0; i < lst->size; i++)
        {
            if (lst->data[i] == list)
            {
                return i;
            }
        }
    }
    return -1;
}

function(brl_std_list_get)
{
    Int list = arg_i(0);
    Int index = arg_i(1);
    if (arg_t(0) == TYPE_LIST)
    {
        IntList *lst = (IntList*)data(list).pointer;
        return lst->data[index];
    }
    return -1;
}

function(brl_std_list_length)
{
    Int list = arg_i(0);
    return -1;
}

function(brl_std_list_last)
{
    Int list = arg_i(0);
    if (arg_t(0) == TYPE_LIST)
    {
        IntList *lst = (IntList*)data(list).pointer;
        return lst->data[lst->size - 1];
    }
    return -1;
}

// std string

function(brl_std_string_concat)
{
    Int _newstr = new_string(vm, "");
    char* newstr = data(_newstr).string;
    for (Int i = 0; i < args->size; i++)
    {
        if (vm->typestack->data[args->data[i]] == TYPE_STRING)
        {
            strcat(newstr, vm->stack->data[args->data[i]].string);
        }
    }
    return _newstr;
}

function(brl_std_string_find)
{
    return new_number(vm, str_find(arg(0).string, arg(1).string));
}

function(brl_std_string_ndup)
{
    char* _str = str_nduplicate(arg(0).string, arg(1).number);
    Int result = new_string(vm, _str);
    return result;
}

function(brl_std_string_split)
{
    Int _splited = new_list(vm);
    IntList *__splited = (IntList*)data(_splited).pointer;
    char* str = arg(0).string;
    char* separator = arg(1).string;
    StringList *splited = str_split(str, separator);
    for (Int i = 0; i < splited->size; i++)
    {
        Int _str = new_string(vm, splited->data[i]);
        stack_push(*__splited, _str);
    }
    stack_free(*splited);
    return _splited;
}

function(brl_std_string_replace)
{
    Int str = arg_i(0);
    Int substr = arg_i(1);
    Int replacement = arg_i(2);
    char* _str = data(str).string;
    char* _substr = data(substr).string;
    char* _replacement = data(replacement).string;
    char* _newstr = str_replace(_str, _substr, _replacement);
    Int result = new_string(vm, _newstr);
    free(_newstr);
    return result;
}

function(brl_std_string_replace_all)
{
    Int str = arg_i(0);
    Int substr = arg_i(1);
    Int replacement = arg_i(2);
    char* _str = data(str).string;
    char* _substr = data(substr).string;
    char* _replacement = data(replacement).string;
    char* _newstr = str_replace_all(_str, _substr, _replacement);
    Int result = new_string(vm, _newstr);
    free(_newstr);
    return result;
}

function(brl_std_string_to_number)
{
    Int str = arg_i(0);
    return new_number(vm, atof(data(str).string));
}

function(brl_std_string_length)
{
    Int str = arg_i(0);
    return new_number(vm, strlen(data(str).string));
}

function(brl_std_string_format)
{
    Int str = stack_shift(*args);
    Int result = -1;
    char* _str = str_duplicate(data(str).string);
    for (Int i = 0; i < strlen(_str); i++)
    {
        if (_str[i] == '%')
        {
            if (_str[i+1] == 'd')
            {
                Int value = stack_shift(*args);
                char* _value = str_format("%ld", (Int)data(value).number);
                char* _newstr = str_replace(_str, "\%d", _value);
                free(_str);
                _str = _newstr;
            }
            else if (_str[i+1] == 's')
            {
                Int value = stack_shift(*args);
                char* _value = data(value).string;
                char* _newstr = str_replace(_str, "\%s", _value);
                free(_str);
                _str = _newstr;
            }
            else if (_str[i+1] == 'f')
            {
                Int value = stack_shift(*args);
                char* _value = str_format("%f", data(value).number);
                char* _newstr = str_replace(_str, "\%f", _value);
                free(_str);
                _str = _newstr;
            }
            else if (_str[i+1] == 'p')
            {
                Int value = stack_shift(*args);
                char* _value = str_format("%p", data(value).pointer);
                char* _newstr = str_replace(_str, "\%p", _value);
                free(_str);
                _str = _newstr;
            }
        }
        else if (_str[i] == '\\')
        {
            if (_str[i+1] == 'n')
            {
                char* _newstr = str_replace(_str, "\\n", "\n");
                free(_str);
                _str = _newstr;
            }
            else if (_str[i+1] == 't')
            {
                char* _newstr = str_replace(_str, "\\t", "\t");
                free(_str);
                _str = _newstr;
            }
            else if (_str[i+1] == 'r')
            {
                char* _newstr = str_replace(_str, "\\r", "\r");
                free(_str);
                _str = _newstr;
            }
            else if (_str[i+1] >= '0' && _str[i+1] <= '9')
            {
                char* _newstr = str_format("%s%c", _str + i + 1, (char)atoi(_str + i + 1));
                free(_str);
                _str = _newstr;
            }
        }
    }
    result = new_var(vm);
    data(result).string = _str;
    data_t(result) = TYPE_STRING;
    return result;
}

// std conditions
// std conditions
// std conditions
function(brl_std_condition_equals)
{
    if (arg_t(0) == arg_t(1))
    {
        if (arg_t(0) == TYPE_NUMBER)
        {
            return new_number(vm, arg(0).number == arg(1).number);
        }
        else if (arg_t(0) == TYPE_STRING)
        {
            return new_number(vm, strcmp(arg(0).string, arg(1).string) == 0);
        }
    }
    return (new_number(vm, 0));
}

function(brl_std_condition_not_equals)
{
    if (arg_t(0) == arg_t(1))
    {
        if (arg_t(0) == TYPE_NUMBER)
        {
            return new_number(vm, arg(0).number != arg(1).number);
        }
        else if (arg_t(0) == TYPE_STRING)
        {
            return new_number(vm, strcmp(arg(0).string, arg(1).string) != 0);
        }
    }
    return (new_number(vm, 0));
}

function(brl_std_condition_greater)
{
    if (arg_t(0) == arg_t(1))
    {
        if (arg_t(0) == TYPE_NUMBER)
        {
            return new_number(vm, arg(0).number > arg(1).number);
        }
        else if (arg_t(0) == TYPE_STRING)
        {
            return new_number(vm, strcmp(arg(0).string, arg(1).string) > 0);
        }
    }
    return (new_number(vm, 0));
}

function(brl_std_condition_less)
{
    if (arg_t(0) == arg_t(1))
    {
        if (arg_t(0) == TYPE_NUMBER)
        {
            return new_number(vm, arg(0).number < arg(1).number);
        }
        else if (arg_t(0) == TYPE_STRING)
        {
            return new_number(vm, strcmp(arg(0).string, arg(1).string) < 0);
        }
    }
    return (new_number(vm, 0));
}

function(brl_std_condition_greater_equals)
{
    if (arg_t(0) == arg_t(1))
    {
        if (arg_t(0) == TYPE_NUMBER)
        {
            return new_number(vm, arg(0).number >= arg(1).number);
        }
        else if (arg_t(0) == TYPE_STRING)
        {
            return new_number(vm, strcmp(arg(0).string, arg(1).string) >= 0);
        }
    }
    return (new_number(vm, 0));
}

function(brl_std_condition_less_equals)
{
    if (arg_t(0) == arg_t(1))
    {
        if (arg_t(0) == TYPE_NUMBER)
        {
            return new_number(vm, arg(0).number <= arg(1).number);
        }
        else if (arg_t(0) == TYPE_STRING)
        {
            return new_number(vm, strcmp(arg(0).string, arg(1).string) <= 0);
        }
    }
    return (new_number(vm, 0));
}

function(brl_std_condition_and)
{
    return new_number(vm, (is_true(arg(0), arg_t(0)) && is_true(arg(1), arg_t(1))));
}

function(brl_std_condition_or)
{
    Int result = 0;
    if (is_true(arg(0), arg_t(0)))
    {
        Int index = new_var(vm);
        if (arg_t(0) == TYPE_LIST || arg_t(0) == TYPE_STRING)
        {
            data(index) = arg(0);
        }
        else
        {
            data(index) = value_duplicate(arg(0), arg_t(0));
        }
        arg_t(0) = arg_t(1);
        result = index;
    }
    else
    {
        Int index = new_var(vm);
        if (arg_t(1) == TYPE_LIST || arg_t(1) == TYPE_STRING)
        {
            data(index) = arg(1);
        }
        else
        {
            data(index) = value_duplicate(arg(1), arg_t(1));
        }
        arg_t(0) = arg_t(1);
        result = index;
    }
    return result;
}

function(brl_std_condition_not)
{
    Int result = 0;
    if (arg_t(0) == TYPE_NUMBER)
    {
        result = new_number(vm, !arg(0).number);
    }
    else if (is_true(arg(0), arg_t(0)))
    {
        result = new_number(vm, 0);
    }
    else
    {
        result = new_number(vm, 1);
    }

    return result;
}

function(brl_std_condition_if)
{
    Int result = eval(vm, arg(0).string);
    if (is_true(data(result), data_t(result)))
    {
        result = eval(vm, arg(1).string);
        return result;
    }
    unuse_var(vm, result);
    return -1;
}

function(brl_std_condition_ifelse)
{
    Int result = eval(vm, arg(0).string);
    if (is_true(data(result), data_t(result)))
    {
        result = eval(vm, arg(1).string);
    }
    else
    {
        result = eval(vm, arg(2).string);
    }
    return result;
}

// std loop
// std loop
// std loop

function(brl_std_loop_while)
{
    Int condition = eval(vm, arg(0).string);
    while (is_true(data(condition), data_t(condition)))
    {
        eval(vm, arg(1).string);
        condition = eval(vm, arg(0).string);
    }
    return -1;
}

function(brl_std_loop_repeat)
{
    for (Int i = 0; i < arg(0).number; i++)
    {
        eval(vm, arg(1).string);
    }
    return -1;
}

function(brl_std_loop_each)
{
    IntList *list = (IntList*)arg(0).pointer;
    for (Int i = 0; i < list->size; i++)
    {
        hash_set(vm, arg(1).string, list->data[i]);
        eval(vm, arg(2).string);
    }
    return -1;
}

// memory functions
function(brl_mem_clear)
{
    for (Int index = 0; index < vm->stack->size; index++)
    {
        if (stack_find(*vm->temp, index) >= 0)
        {
            unuse_var(vm, index);
        }
    }

    return -1;
}

function(brl_mem_hold)
{
    for (Int i = 0; i < args->size; i++)
    {
        hold_var(vm, args->data[i]);
    }
    
    return -1;
}

function(brl_mem_keep)
{
    hold_var(vm, arg_i(0));
    return arg_i(0);
}

function(brl_mem_unhold)
{
    for (Int i = 0; i < args->size; i++)
    {
        unhold_var(vm, arg_i(i));
    }
    return -1;
}

function(brl_mem_rebase)
{
    Int index = vm->stack->size - 1;
    while (vm->unused->size > 0)
    {
        index = stack_pop(*vm->unused);
        // removes
        for (Int i = 0; i < vm->hashes->size; i++)
        {
            if (hash(i).index == index)
            {
                free(hash(i).key);
                stack_remove(*vm->hashes, i);
            }
        }
        for (Int i = 0; i < vm->unused->size; i++)
        {
            if (vm->unused->data[i] == index)
            {
                stack_remove(*vm->unused, i);
            }
        }
        for (Int i = 0; i < vm->temp->size; i++)
        {
            if (vm->temp->data[i] == index)
            {
                stack_remove(*vm->temp, i);
            }
        }
        for (Int i = 0; i < vm->stack->size; i++)
        {
            if (data_t(i) == TYPE_LIST)
            {
                IntList *list = (IntList*)data(i).pointer;
                for (Int j = 0; j < list->size; j++)
                {
                    if (list->data[j] == index)
                    {
                        list->data[j] = -1;
                    }
                    else if (list->data[j] > index)
                    {
                        list->data[j]--;
                    }
                }
            }
        }

        // updates
        for (Int i = 0; i < vm->temp->size; i++)
        {
            if (vm->temp->data[i] == index)
            {
                vm->temp->data[i] = -1;
            }
            else if (vm->temp->data[i] > index)
            {
                vm->temp->data[i]--;
            }
        }
        for (Int i = 0; i < vm->unused->size; i++)
        {
            if (vm->unused->data[i] == index)
            {
                vm->unused->data[i] = -1;
            }
            else if (vm->unused->data[i] > index)
            {
                vm->unused->data[i]--;
            }
        }
        for (Int i = 0; i < vm->hashes->size; i++)
        {
            if (hash(i).index == index)
            {
                hash(i).index = -1;
            }
            else if (hash(i).index > index)
            {
                hash(i).index--;
            }
        }

        stack_remove(*vm->stack, index);
        stack_remove(*vm->typestack, index);
    }

    return -1;
}

function(brl_mem_collect)
{
    collect_garbage(vm);
    return -1;
}

function(brl_mem_swap)
{
    if (args->size == 2)
    {
        if (arg_i(0) >= 0 && arg_i(0) < vm->stack->size && arg_i(1) >= 0 && arg_i(1) < vm->stack->size)
        {
            Value temp = arg(0);
            char type = arg_t(0);
            arg(0) = arg(1);
            arg_t(0) = arg_t(1);
            arg(1) = temp;
            arg_t(1) = type;
        }
    }
    return -1;
}

function(brl_mem_set)
{
    if (arg_i(0) >= 0 && arg_i(0) < vm->stack->size)
    {
        Int found = stack_find(*vm->temp, arg_i(0));
        if (found >= 0)
        {
            unuse_var(vm, arg_i(0));
        }
        arg(0) = arg(1);
        arg_t(0) = arg_t(1);
    }
    
    return -1;
}

function(brl_mem_use)
{
    for (Int i = 0; i < args->size; i++)
    {
        use_var(vm, arg_i(i));
    }
    return -1;
}

function(brl_mem_unuse)
{
    for (Int i = 0; i < args->size; i++)
    {
        unuse_var(vm, arg_i(i));
    }
    return -1;
}

function(brl_mem_copy)
{
    Int newvar = new_var(vm);
    data(newvar) = value_duplicate(data(arg_i(0)), data_t(arg_i(0)));
    return newvar;
}

function(brl_mem_get)
{
    if (arg(0).number >= 0 && arg(0).number < vm->stack->size)
    {
        return arg_i(0);
    }
    
    return -1;
}

function(brl_mem_delete)
{
    for (Int i = 0; i < args->size; i++)
    {
        unuse_var(vm, arg_i(i));
    }
    return -1;
}


function(brl_mem_length)
{
    return new_number(vm, vm->stack->size);
}

function(brl_mem_next)
{
    if (arg_i(0) < 0)
    {
        return -1;
    }
    
    stack_unshift(*vm->unused, arg_i(0));
    
    while (args->size > 0)
    {
        stack_unshift(*vm->unused, arg_i(1));
    }

    return -1;
}

function(brl_mem_sector_new)
{
    Int index = new_var(vm);
    while ((arg(0).number - 1) > 0)
    {
        new_var(vm);
        arg(0).number--;
    }
    return index;
}

function(brl_mem_sector_copy)
{
    while (arg(2).number > 0)
    {
        if (arg_i(0) >= 0 && arg_i(0) < vm->stack->size)
        {
            if (arg_i(1) >= 0 && arg_i(1) < vm->stack->size)
            {
                arg(1) = value_duplicate(arg(0), arg_t(0));
                arg_t(1) = arg_t(0);
            }
        }
        arg_i(0)++;
        arg_i(1)++;
        arg(2).number--;
    }
    return -1;
}

function(brl_mem_sector_swap)
{
    while (arg(2).number > 0)
    {
        if (arg_i(0) >= 0 && arg_i(0) < vm->stack->size)
        {
            if (arg_i(1) >= 0 && arg_i(1) < vm->stack->size)
            {
                Value temp = arg(0);
                char type = arg_t(0);
                arg(0) = arg(1);
                arg_t(0) = arg_t(1);
                arg(1) = temp;
                arg_t(1) = type;
            }
        }
        arg_i(0)++;
        arg_i(1)++;
        arg(2).number--;
    }
    return -1;
}


function(brl_std_mem_push)
{
    while (args->size > 0)
    {
        stack_push(*vm->stack, arg(0));
        stack_push(*vm->typestack, arg_t(0));
    }
    return -1;
}

function(brl_std_mem_unshift)
{
    for (Int i = args->size - 1; i >= 0; i--)
    {
        stack_unshift(*vm->stack, arg(i));
        stack_unshift(*vm->typestack, arg_t(i));
    }

    for (Int i = 0; i < vm->hashes->size; i++)
    {
        hash(i).index += args->size;
    }

    for (Int i = 0; i < vm->temp->size; i++)
    {
        data_temp(i) += args->size;
    }

    for (Int i = 0; i < vm->unused->size; i++)
    {
        data_unused(i) += args->size;
    }
    
    return -1;
}

function(brl_std_mem_pop)
{
    for (Int i = 0; i < arg(0).number; i++)
    {
        unuse_var(vm, vm->stack->size-1);
        stack_pop(*vm->stack);
        stack_pop(*vm->typestack);
    }
    return -1;
}

function(brl_std_mem_shift)
{
    for (Int i = 0; i < arg(0).number; i++)
    {
        unuse_var(vm, 0);
        stack_shift(*vm->stack);
        stack_shift(*vm->typestack);
    }
    
    for (Int i = 0; i < vm->hashes->size; i++)
    {
        hash(i).index -= arg(0).number;
    }

    for (Int i = 0; i < vm->temp->size; i++)
    {
        vm->temp->data[i] -= arg(0).number;
    }

    for (Int i = 0; i < vm->unused->size; i++)
    {
        vm->unused->data[i] -= arg(0).number;
    }

    return -1;
}

function(brl_std_mem_find)
{
    for (Int i = 0; i < vm->stack->size; i++)
    {
        if (data(i).integer == arg(0).integer)
        {
            return i;
        }
    }
    return -1;
}

// inits
#ifndef ARDUINO
void init_os(VirtualMachine *vm)
{
    register_builtin(vm, "file.read", brl_os_file_read);
    register_builtin(vm, "file.write", brl_os_file_write);
    register_builtin(vm, "file.delete", brl_os_file_delete);
    register_builtin(vm, "file.exists", brl_os_file_exists);
    register_builtin(vm, "file.rename", brl_os_file_rename);
    register_builtin(vm, "file.copy", brl_os_file_copy);
    register_builtin(vm, "file.size", brl_os_file_size);

    register_builtin(vm, "os.time", brl_os_time_now);
    register_builtin(vm, "os.clock", brl_os_time_clock);

    register_builtin(vm, "dofile", brl_os_dofile);
    register_builtin(vm, "repl", brl_os_repl);
}
#endif

void init_basics(VirtualMachine *vm)
{
    register_builtin(vm, "#", brl_std_ignore);
    register_builtin(vm, "do", brl_std_do);
    register_builtin(vm, "return", brl_std_return);
    register_builtin(vm, "gindex", brl_std_gindex);
#ifndef ARDUINO
    register_builtin(vm, "ls", brl_std_io_ls);
    register_builtin(vm, "ls.type", brl_std_io_ls_types);
    register_builtin(vm, "ls.hash", brl_std_io_ls_hashes);
    register_builtin(vm, "ls.temp", brl_std_io_ls_temp);
    register_builtin(vm, "ls.free", brl_std_io_ls_unused);
    register_builtin(vm, "print", brl_std_io_print);
#endif
}

void init_type(VirtualMachine *vm)
{
    // types
    register_number(vm, "type.size", sizeof(Value));
    register_number(vm, "type.nil", TYPE_NIL);
    register_number(vm, "type.number", TYPE_NUMBER);
    register_number(vm, "type.integer", TYPE_INTEGER);
    register_number(vm, "type.string", TYPE_STRING);
    register_number(vm, "type.builtin", TYPE_BUILTIN);
    register_number(vm, "type.list", TYPE_LIST);
    register_number(vm, "type.raw", TYPE_RAW);
    register_number(vm, "type.other", TYPE_OTHER);

    // type functions
    register_builtin(vm, "type.get", brl_std_type_get);
    register_builtin(vm, "type.set", brl_std_type_set);
}

void init_loop(VirtualMachine *vm)
{
    register_builtin(vm, "while", brl_std_loop_while);
    register_builtin(vm, "repeat", brl_std_loop_repeat);
    register_builtin(vm, "each", brl_std_loop_each);
}

void init_hash(VirtualMachine *vm)
{
    register_builtin(vm, "hash.set", brl_std_hash_set);
    register_builtin(vm, "hash.get", brl_std_hash_get);
    register_builtin(vm, "hash.delete", brl_std_hash_delete);
    register_builtin(vm, "hash.rename", brl_std_hash_rename);
}

void init_math(VirtualMachine *vm)
{
    register_builtin(vm, "add", brl_std_math_add);
    register_builtin(vm, "sub", brl_std_math_sub);
    register_builtin(vm, "mul", brl_std_math_mul);
    register_builtin(vm, "div", brl_std_math_div);
    register_builtin(vm, "mod", brl_std_math_mod);
    register_builtin(vm, "pow", brl_std_math_pow);
    register_builtin(vm, "abs", brl_std_math_abs);
    register_builtin(vm, "sqrt", brl_std_math_sqrt);
    register_builtin(vm, "ceil", brl_std_math_ceil);
    register_builtin(vm, "seed", brl_std_math_seed);
    register_builtin(vm, "floor", brl_std_math_floor);
    register_builtin(vm, "round", brl_std_math_round);
    register_builtin(vm, "random", brl_std_math_random);
    register_builtin(vm, "incr", brl_std_math_increment);
    register_builtin(vm, "decr", brl_std_math_decrement);
    register_builtin(vm, "int.from.float", int_from_float);
    register_builtin(vm, "float.from.int", int_to_float);
}

void init_string(VirtualMachine *vm)
{
    register_builtin(vm, "string.sub", brl_std_string_ndup);
    register_builtin(vm, "string.find", brl_std_string_find);
    register_builtin(vm, "string.len", brl_std_string_length);
    register_builtin(vm, "string.split", brl_std_string_split);
    register_builtin(vm, "string.concat", brl_std_string_concat);
    register_builtin(vm, "string.format", brl_std_string_format);
    register_builtin(vm, "string.replace", brl_std_string_replace);
    register_builtin(vm, "string.to.number", brl_std_string_to_number);
    register_builtin(vm, "string.replace.all", brl_std_string_replace_all);
}

void init_condition(VirtualMachine *vm)
{
    register_builtin(vm, "or", brl_std_condition_or);
    register_builtin(vm, "if", brl_std_condition_if);
    register_builtin(vm, "<", brl_std_condition_less);
    register_builtin(vm, "and", brl_std_condition_and);
    register_builtin(vm, "not", brl_std_condition_not);
    register_builtin(vm, "==", brl_std_condition_equals);
    register_builtin(vm, ">", brl_std_condition_greater);
    register_builtin(vm, "ifelse", brl_std_condition_ifelse);
    register_builtin(vm, "!=", brl_std_condition_not_equals);
    register_builtin(vm, "<=", brl_std_condition_less_equals);
    register_builtin(vm, ">=", brl_std_condition_greater_equals);
}

void init_list(VirtualMachine *vm)
{
    register_builtin(vm, "list.new", brl_std_list_new);
    register_builtin(vm, "list.pop", brl_std_list_pop);
    register_builtin(vm, "list.get", brl_std_list_get);
    register_builtin(vm, "list.len", brl_std_list_length);
    register_builtin(vm, "list.push", brl_std_list_push);
    register_builtin(vm, "list.find", brl_std_list_find);
    register_builtin(vm, "list.shift", brl_std_list_shift);
    register_builtin(vm, "list.insert", brl_std_list_insert);
    register_builtin(vm, "list.remove", brl_std_list_remove);
    register_builtin(vm, "list.concat", brl_std_list_concat);
    register_builtin(vm, "list.unshift", brl_std_list_unshift);
    register_builtin(vm, "list.last", brl_std_list_last);
}

void init_sector(VirtualMachine *vm)
{
    register_builtin(vm, "sector.new", brl_mem_sector_new);
    register_builtin(vm, "sector.copy", brl_mem_sector_copy);
    register_builtin(vm, "sector.swap", brl_mem_sector_swap);
}

void init_mem(VirtualMachine *vm)
{
    register_number(vm, "mem.size", sizeof(Float));
    register_builtin(vm, "mem.use", brl_mem_use);
    register_builtin(vm, "mem.unuse", brl_mem_unuse);
    register_builtin(vm, "mem.get", brl_mem_get);
    register_builtin(vm, "mem.hold", brl_mem_hold);
    register_builtin(vm, "mem.keep", brl_mem_keep);
    register_builtin(vm, "mem.set", brl_mem_set);
    register_builtin(vm, "mem.copy", brl_mem_copy);
    register_builtin(vm, "mem.len", brl_mem_length);
    register_builtin(vm, "mem.clear", brl_mem_clear);
    register_builtin(vm, "mem.rebase", brl_mem_rebase);
    register_builtin(vm, "mem.unhold", brl_mem_unhold);
    register_builtin(vm, "mem.delete", brl_mem_delete);
    register_builtin(vm, "mem.collect", brl_mem_collect);
    register_builtin(vm, "mem.swap", brl_mem_swap);
    register_builtin(vm, "mem.next", brl_mem_next);
    register_builtin(vm, "mem.push", brl_std_mem_push);
    register_builtin(vm, "mem.unshift", brl_std_mem_unshift);
    register_builtin(vm, "mem.pop", brl_std_mem_pop);
    register_builtin(vm, "mem.shift", brl_std_mem_shift);
    register_builtin(vm, "mem.find", brl_std_mem_find);

    init_sector(vm);
}

// std init presets
void init_std(VirtualMachine *vm)
{
    #ifndef ARDUINO
    init_os(vm);
    #endif
    init_basics(vm);
    init_type(vm);
    init_loop(vm);
    init_hash(vm);
    init_list(vm);
    init_math(vm);
    init_string(vm);
    init_condition(vm);
    init_mem(vm);
    register_string(vm, "VERSION", VERSION);// version
}