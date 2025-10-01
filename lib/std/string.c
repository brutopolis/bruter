#include "bruter.h"

void feraw_strdup(BruterList* stack)
{
    char* str = bruter_pop_pointer(stack);
    if (str == NULL)
    {
        bruter_push_pointer(stack, NULL, NULL, BRUTER_TYPE_ANY);
        return;
    }
    
    char* new_str = strdup(str);
    if (new_str == NULL)
    {
        printf("BRUTER_ERROR: failed to duplicate string\n");
        exit(EXIT_FAILURE);
    }

    bruter_push_pointer(stack, new_str, NULL, BRUTER_TYPE_BUFFER);
}

void feraw_strcat(BruterList* stack)
{
    char* str1 = bruter_pop_pointer(stack);
    char* str2 = bruter_pop_pointer(stack);
    
    if (str1 == NULL || str2 == NULL)
    {
        bruter_push_pointer(stack, NULL, NULL, BRUTER_TYPE_ANY);
        return;
    }
    
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    
    char* new_str = (char*)malloc(len1 + len2 + 1);
    if (new_str == NULL)
    {
        printf("BRUTER_ERROR: failed to allocate memory for concatenated string\n");
        exit(EXIT_FAILURE);
    }
    
    strcpy(new_str, str1);
    strcat(new_str, str2);
    
    bruter_push_pointer(stack, new_str, NULL, BRUTER_TYPE_BUFFER);
}

void feraw_strcpy(BruterList* stack)
{
    char* dest = bruter_pop_pointer(stack);
    char* src = bruter_pop_pointer(stack);
    
    if (src == NULL || dest == NULL)
    {
        bruter_push_pointer(stack, NULL, NULL, BRUTER_TYPE_ANY);
        return;
    }
    
    strcpy(dest, src);
    bruter_push_pointer(stack, dest, NULL, BRUTER_TYPE_BUFFER);
}

void feraw_strncpy(BruterList* stack)
{
    char* dest = bruter_pop_pointer(stack);
    char* src = bruter_pop_pointer(stack);
    BruterInt n = bruter_pop_int(stack);
    
    if (src == NULL || dest == NULL)
    {
        bruter_push_pointer(stack, NULL, NULL, BRUTER_TYPE_ANY);
        return;
    }
    
    strncpy(dest, src, n);
    dest[n] = '\0'; // Ensure null termination
    
    bruter_push_pointer(stack, dest, NULL, BRUTER_TYPE_BUFFER);
}

void feraw_strcmp(BruterList* stack)
{
    char* str1 = bruter_pop_pointer(stack);
    char* str2 = bruter_pop_pointer(stack);
    
    if (str1 == NULL || str2 == NULL)
    {
        bruter_push_int(stack, 0, NULL, BRUTER_TYPE_ANY);
        return;
    }
    
    int result = strcmp(str1, str2);
    bruter_push_int(stack, result, NULL, BRUTER_TYPE_ANY);
}

void feraw_strlen(BruterList* stack)
{
    char* str = bruter_pop_pointer(stack);
    
    if (str == NULL)
    {
        bruter_push_int(stack, 0, NULL, BRUTER_TYPE_ANY);
        return;
    }
    
    BruterInt length = strlen(str);
    bruter_push_int(stack, length, NULL, BRUTER_TYPE_ANY);
}

void feraw_strchr(BruterList* stack)
{
    char* str = bruter_pop_pointer(stack);
    char c = (char)bruter_pop_int(stack);

    char* result = strchr(str, c);
    bruter_push_pointer(stack, result, NULL, BRUTER_TYPE_BUFFER);
}

void feraw_strstr(BruterList* stack)
{
    char* str1 = bruter_pop_pointer(stack);
    char* str2 = (char*)bruter_pop_int(stack);

    char* result = strstr(str1, str2);
    bruter_push_pointer(stack, result, NULL, BRUTER_TYPE_BUFFER);
}

void feraw_strsplit(BruterList* stack)
{
    char* original_str = (char*)bruter_pop_pointer(stack);
    char* delimiters = (char*)bruter_pop_pointer(stack);

    BruterList* result = bruter_new(BRUTER_DEFAULT_SIZE, false, true);
    for (char* token = strtok(original_str, delimiters); token != NULL; token = strtok(NULL, delimiters))
    {
        bruter_push_pointer(result, strdup(token), NULL, BRUTER_TYPE_BUFFER);
    }
    bruter_push_pointer(stack, result, NULL, BRUTER_TYPE_LIST);
}