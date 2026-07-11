#include "codexion.h"

long    get_time_ms()
{
    struct timeval  tv;
    long            time;

    gettimeofday(&tv, NULL);
    time = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
    return (time);
}

long    get_elapsed_ms(long start)
{
    return (get_time_ms() - start);
}
