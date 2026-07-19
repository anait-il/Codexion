/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anait-il <your@mail.com>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 14:30:48 by anait-il          #+#    #+#             */
/*   Updated: 2026/07/17 14:49:02 by anait-il         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int detect_end_compile(t_program *program)
{
    int i;

    i = 0;
    //pthread_mutex_lock(&program->monitor_lock);
    while (i < program->data.number_of_coders)
    {
        if (program->coders[i].compile_counter < program->data.number_of_compiles_required)
            return (1);
        i++;
    }
    //pthread_mutex_unlock(&program->monitor_lock);
    return (0);
}

static int    detect_burnout(t_program *program)
{
    int     i;
    long    now;

    i = 0;
    //pthread_mutex_lock(&program->monitor_lock);
    now  = get_time_ms();
    while (i < program->data.number_of_coders)
    {
        if ((now - program->coders[i].last_compile_time) > program->data.time_to_burnout)
            return (i + 1);
        i++;
    }
    //pthread_mutex_unlock(&program->monitor_lock);
    return (0);
}

static void    *monitor_routine(void *arg)
{
    t_program   *program;
    int         i;
    int         state;
    long        now;

    now = get_time_ms();
    program = (t_program*)arg;
    printf("monitor start at %ld\n", now - program->start_time);
    i = 0;
    pthread_mutex_lock(&program->monitor_lock);
    program->running = true;
    pthread_mutex_unlock(&program->monitor_lock);
    while (true)
    {
        printf("inside the monitor ###############################################\n");
        state = detect_burnout(program);
        if (state)
        {
            pthread_mutex_lock(&program->monitor_lock);
            program->running = false;
            pthread_mutex_lock(&program->monitor_lock);
            log_burnout(program, state);
            break;
        }
        if (!detect_end_compile(program))
        {
            pthread_mutex_lock(&program->monitor_lock);
            program->running = false;
            pthread_mutex_unlock(&program->monitor_lock);
            break;
        }
        usleep(1000);
    }
    pthread_mutex_lock(&program->monitor_lock);
    program->running = false;
    pthread_mutex_unlock(&program->monitor_lock);
    return (NULL);
}

int start_monitoring(t_program *program)
{
    int status;

    status = pthread_create(&program->monitor, NULL, monitor_routine, program);
    if (status)
    {
        free_dongles(program);
        return (status);
    }
    status = pthread_join(program->monitor, NULL);
    return (0);
}
