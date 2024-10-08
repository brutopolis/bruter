#ifndef BRUTER_H
#define BRUTER_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdarg.h>

#ifndef ARDUINO
#include <poll.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#ifdef _WIN32
#include <windows.h>
// Código específico para Windows
#else // Linux
#include <unistd.h>
#include <sys/wait.h>
extern char** environ;
#endif
#endif

#define VERSION "0.5.7b"

#define TYPE_NIL 0
#define TYPE_NUMBER 1
#define TYPE_STRING 2
#define TYPE_LIST 3
#define TYPE_BUILTIN 4
#define TYPE_FUNCTION 5
#define TYPE_PROCESS 6
#define TYPE_THREAD 7
#define TYPE_OTHER 8


#if defined(__x86_64__) || defined(_M_X64) || defined(__aarch64__) || defined(__LP64__) || defined(_WIN64) || defined(__amd64__) || defined(__x86_64) || defined(__x86_64__) || defined(__x86_64)
    #define Int long
    #define Float double
#elif defined(__i386__) || defined(_M_IX86) || defined(__ARM_ARCH_7__)
    #define Int int
    #define Float float
#elif defined(__AVR__)
    #define Int short
    // maybe i should implement a 16-bit float in far future
    #define Float float 
#elif defined(ESP8266) || defined(ESP32)
    #define Int int
    #define Float float
#else
    #define Int int
    #define Float float
#endif

#define Nil -1

#ifndef __null
#define __null 0
#endif

#ifndef NULL
#define NULL 0
#endif

// process type
typedef struct {
    int parent_to_child[2]; // Pipe para comunicação: Pai -> Filho
    int child_to_parent[2]; // Pipe para comunicação: Filho -> Pai
    pid_t pid;              // PID do processo filho
} process_t;


#define is_space(c) (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f')

//stack implementation
#define Stack(T) struct \
{ \
    T *data; \
    int size; \
    int capacity; \
}

#define stack_init(s) do \
{ \
    (s).data = NULL; \
    (s).size = 0; \
    (s).capacity = 0;\
} while (0)

#define stack_push(s, v) do { \
    if ((s).size == (s).capacity) { \
        (s).capacity = (s).capacity == 0 ? 1 : (s).capacity * 2; \
        (s).data = realloc((s).data, (s).capacity * sizeof(*(s).data)); \
    } \
    (s).data[(s).size++] = (v); \
} while (0)

#define stack_unshift(s, v) do { \
    if ((s).size == (s).capacity) { \
        (s).capacity = (s).capacity == 0 ? 1 : (s).capacity * 2; \
        (s).data = realloc((s).data, (s).capacity * sizeof(*(s).data)); \
    } \
    for (int i = (s).size; i > 0; i--) { \
        (s).data[i] = (s).data[i - 1]; \
    } \
    (s).data[0] = (v); \
    (s).size++; \
} while (0)

#define stack_pop(s) ((s).data[--(s).size])

#define stack_shift(s) ({ \
    typeof((s).data[0]) ret = (s).data[0]; \
    for (int i = 0; i < (s).size - 1; i++) { \
        (s).data[i] = (s).data[i + 1]; \
    } \
    (s).size--; \
    ret; \
})

#define stack_free(s) ({free((s).data);free(&s);})

//swap elements from index i1 to index i2
#define stack_move(s, i1, i2) do { \
    typeof((s).data[i1]) tmp = (s).data[i1]; \
    (s).data[i1] = (s).data[i2]; \
    (s).data[i2] = tmp; \
} while (0)

//insert element v at index i
#define stack_insert(s, i, v) do { \
    if ((s).size == (s).capacity) { \
        (s).capacity = (s).capacity == 0 ? 1 : (s).capacity * 2; \
        (s).data = realloc((s).data, (s).capacity * sizeof(*(s).data)); \
    } \
    for (int j = (s).size; j > i; j--) { \
        (s).data[j] = (s).data[j - 1]; \
    } \
    (s).data[i] = (v); \
    (s).size++; \
} while (0)

//remove element at index i and return it
#define stack_remove(s, i) ({ \
    typeof((s).data[i]) ret = (s).data[i]; \
    for (int j = i; j < (s).size - 1; j++) { \
        (s).data[j] = (s).data[j + 1]; \
    } \
    (s).size--; \
    ret; \
})

//Value
typedef union 
{
    Float number;
    Int integer;
    char* string;
    void* pointer;
    process_t* process;
} Value;

//Hash
typedef struct
{
    char *key;
    Int index;
} Hash;

//List
typedef Stack(Value) ValueList;
typedef Stack(Hash) HashList;
typedef Stack(char*) StringList;
typedef Stack(Int) IntList;
typedef Stack(char) CharList;
// mutex stack
typedef Stack(pthread_mutex_t) MutexList;

typedef struct
{
    ValueList *stack;
    CharList *typestack;
    HashList *hashes;
    IntList *unused;
    IntList *temp;
} VirtualMachine;

typedef struct
{
    ValueList* stack;
    CharList* typestack;
} ThreadTransfer;

typedef struct
{
    VirtualMachine* vm;
    StringList* strings;
    pthread_mutex_t* strings_lock;
    pthread_t* thread;
    volatile int status;         // precisa ser volátil, se não trava a criação de threads 
    pthread_mutex_t* thread_lock;
    ThreadTransfer* transfer;
} Thread;

//Function
typedef Int (*Function)(VirtualMachine*, IntList*);

//String
char* str_duplicate(const char *str);
char* str_nduplicate(const char *str, Int n);
char* str_format(const char *fmt, ...);
char* str_sub(const char *str, Int start, Int end);
char* str_concat(const char *str1, const char *str2);
Int str_find(const char *str, const char *substr);
char* str_replace(const char *str, const char *substr, const char *replacement);
char* str_replace_all(const char *str, const char *substr, const char *replacement);

StringList* splitString(char *str, char *delim);
StringList* special_space_split(char *str);

char is_true(Value value, char __type);
#define is_space(c) (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f')

// variable

ValueList* make_value_list();
IntList* make_int_list();
StringList* make_string_list();
CharList* make_char_list();
Thread* make_thread_arg(VirtualMachine* vm, char* str, ...);
VirtualMachine* make_vm();
void free_vm(VirtualMachine *vm);
void free_var(VirtualMachine *vm, Int index);
void unuse_var(VirtualMachine *vm, Int index);

Int new_number(VirtualMachine *vm, Float number);
Int new_string(VirtualMachine *vm, char *str);
Int new_builtin(VirtualMachine *vm, Function function);
Int new_function(VirtualMachine *vm, char* script);
Int new_list(VirtualMachine *vm);
Int new_var(VirtualMachine *vm);
Int new_thread(VirtualMachine *vm, Thread* thread_arg);

void hold_var(VirtualMachine *vm, Int index);
void unhold_var(VirtualMachine *vm, Int index);

Value value_duplicate(Value value, char type);

Int spawn_var(VirtualMachine *vm, char* varname);
Int spawn_string(VirtualMachine *vm, char* varname, char* string);
Int spawn_number(VirtualMachine *vm, char* varname, Float number);
Int spawn_builtin(VirtualMachine *vm, char* varname, Function function);
Int spawn_function(VirtualMachine *vm, char* varname, char* script);
Int spawn_list(VirtualMachine *vm, char* varname);

Int hash_find(VirtualMachine *vm, char *key);
void hash_set(VirtualMachine *vm, char *key, Int index);
void hash_unset(VirtualMachine *vm, char *key);

// eval
Int eval(VirtualMachine *vm, char *cmd);
Int interpret(VirtualMachine *vm, char* cmd);
IntList* parse(VirtualMachine *vm, char *cmd);
void collect_garbage(VirtualMachine *vm);

void init_default_vars(VirtualMachine *vm);
void init_std(VirtualMachine *vm);
void init_math(VirtualMachine *vm);
void init_list(VirtualMachine *vm);
void init_string(VirtualMachine *vm);
void init_condition(VirtualMachine *vm);
void init_prototype(VirtualMachine *vm);
void init_type(VirtualMachine *vm);
void init_hash(VirtualMachine *vm);
void init_loop(VirtualMachine *vm);

#ifndef ARDUINO

void init_std_os(VirtualMachine *vm);
char* readfile(char *filename);
void writefile(char *filename, char *content);

#ifndef _WIN32
void init_linux(VirtualMachine *vm);
#else 
void init_windows(VirtualMachine *vm);
#endif

// multiprocess function declarations
int fork_process(process_t* process, void (*child_function)(process_t*, VirtualMachine*), VirtualMachine* vm);
int create_process(process_t* process, char* argv[]);
void send_dynamic_string(process_t* process, const char* str, int to_parent);
char* receive_dynamic_string(process_t* process, int from_parent);
void process_destroy(process_t* process);

// threads
void* permanent_thread(void* arg);
Int std_thread_create(VirtualMachine* vm, IntList* args);
Int std_thread_join(VirtualMachine* vm, IntList* args);
Int std_thread_send(VirtualMachine* vm, IntList* args);
Int std_thread_destroy(VirtualMachine* vm, IntList* args);
void thread_destroy(Thread* thread_arg);
void init_pthreads(VirtualMachine *vm);

#endif

void preset_all(VirtualMachine *vm);

#endif
