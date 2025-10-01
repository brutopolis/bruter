#include <bruter.h>

#ifdef _WIN32
    #include <windows.h>
    static long long time_ms() {
        static LARGE_INTEGER freq;
        static int initialized = 0;
        if (!initialized) {
            QueryPerformanceFrequency(&freq);
            initialized = 1;
        }
        LARGE_INTEGER counter;
        QueryPerformanceCounter(&counter);
        return (long long)(counter.QuadPart * 1000 / freq.QuadPart);
    }
#else
    #include <time.h>
    static long long time_ms() {
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        return (long long)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
    }
#endif

void feraw_ms(BruterList* stack)
{
    bruter_push_int(stack, time_ms(), NULL, BRUTER_TYPE_ANY);
}

void feraw_time(BruterList* stack)
{
    bruter_push_int(stack, (BruterInt)time(NULL), NULL, BRUTER_TYPE_ANY);
}

void feraw_clock(BruterList* stack)
{
    bruter_push_int(stack, (BruterInt)clock(), NULL, BRUTER_TYPE_ANY);
}
