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


void my_sleep(long time, t_program *program)
{
    long    start;

    start = get_time_ms();
    while (get_time_ms() - start < time)
    {
        usleep((time/10) * 1000);
        if (!is_running(program))
            return;
    }
}