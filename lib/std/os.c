#include <bruter.h>

// file stuff
char* file_read(char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        return NULL;
    }

    char *code = (char*)malloc(1);
    if (code == NULL)
    {
        printf("ERROR: could not allocate memory for file\n");
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
            printf("ERROR: could not reallocate memory while reading file\n");
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

void file_write(char *filename, char *code)
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

void feraw_read_bin(BruterList* stack)
{
    char *filename = bruter_pop_pointer(stack);
    FILE *file = fopen(filename, "rb");
    if (file == NULL)
    {
        bruter_push_pointer(stack, NULL, NULL, BRUTER_TYPE_BUFFER);
        return;
    }
    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);
    uint8_t *buffer = (uint8_t*)malloc(size);
    if (buffer == NULL)
    {
        fclose(file);
        bruter_push_pointer(stack, NULL, NULL, BRUTER_TYPE_BUFFER);
        return;
    }
    fread(buffer, 1, size, file);
    fclose(file);
    bruter_push_pointer(stack, buffer, NULL, BRUTER_TYPE_BUFFER);
}

void feraw_write_bin(BruterList* stack)
{
    char *filename = bruter_pop_pointer(stack);
    uint8_t *buffer = bruter_pop_pointer(stack);
    FILE *file = fopen(filename, "wb");
    if (file == NULL)
    {
        return;
    }
    // Assuming the first 4 bytes of the buffer contain the size
    uint32_t size = *((uint32_t*)buffer);
    fwrite(buffer + 4, 1, size, file);
    fclose(file);
}

void feraw_read_file(BruterList* stack)
{
    char *filename = bruter_pop_pointer(stack);
    char *content = file_read(filename);
    bruter_push_pointer(stack, content, NULL, BRUTER_TYPE_BUFFER);
}

void feraw_write_file(BruterList* stack)
{
    char *filename = bruter_pop_pointer(stack);
    char *content = bruter_pop_pointer(stack);
    file_write(filename, content);
}

void feraw_file_exists(BruterList* stack)
{
    char *filename = bruter_pop_pointer(stack);
    bool exists = file_exists(filename);
    bruter_push_int(stack, exists ? 1 : 0, NULL, BRUTER_TYPE_ANY); // Push 1 if exists, 0 if not
}