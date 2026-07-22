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
#include <pthread.h>

static int detect_end_compile(t_program *program)
{
    int i;

    i = 0;
    pthread_mutex_lock(&program->monitor_lock);
    while (i < program->data.number_of_coders)
    {
        if (program->coders[i].compile_counter < program->data.number_of_compiles_required)
        {
            pthread_mutex_unlock(&program->monitor_lock);
            return (1);
        }
        i++;
    }
    pthread_mutex_unlock(&program->monitor_lock);
    return (0);
}

static int    detect_burnout(t_program *program)
{
    int     i;
    long    now;

    i = 0;
    pthread_mutex_lock(&program->monitor_lock);
    now  = get_time_ms();
    while (i < program->data.number_of_coders)
    {
        if ((now - program->coders[i].last_compile_time) > program->data.time_to_burnout)
        {
            pthread_mutex_unlock(&program->monitor_lock);
            return (i + 1);
        }
        i++;
    }
    pthread_mutex_unlock(&program->monitor_lock);
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
    i = 0;
    pthread_mutex_lock(&program->monitor_lock);
    program->running = true;
    pthread_mutex_unlock(&program->monitor_lock);
    while (true)
    {
        state = detect_burnout(program);
        if (state)
        {
            pthread_mutex_lock(&program->monitor_lock);
            log_burnout(program, state);
            break;
        }
        if (!detect_end_compile(program))
        {
            pthread_mutex_lock(&program->monitor_lock);
            break;
        }
        usleep(1000);
    }
    stop_simulation(program);
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
    return (0);
}

void    stop_simulation(t_program *program)
{
    int i;

    i = 0;
    if (!program)
        return;
    program->running = false;
    pthread_mutex_unlock(&program->monitor_lock);
    while (i < program->data.number_of_coders)
    {
        pthread_cond_broadcast(&program->dongles[i].cond);
        i++;
    }
}
