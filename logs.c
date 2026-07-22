/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anait-il <your@mail.com>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 14:36:31 by anait-il          #+#    #+#             */
/*   Updated: 2026/07/11 14:36:32 by anait-il         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "codexion.h"

int log_state(t_coder   *coder, char *message)
{
    long    timestamp;

    if (!coder || !message)
        return (1);
    pthread_mutex_lock(&coder->program->monitor_lock);
    if (!coder->program->running)
    {
        pthread_mutex_unlock(&coder->program->monitor_lock);
        return (1);
    }
    pthread_mutex_lock(&coder->program->print_lock);
    timestamp = get_time_ms() - coder->program->start_time;
    printf("%ld %d %s", timestamp, coder->id, message);
    pthread_mutex_unlock(&coder->program->print_lock);
    pthread_mutex_unlock(&coder->program->monitor_lock);
    return (0);
}

void    log_burnout(t_program *program, int coder_id)
{
    long    timestamp;

    if (!program)
        return ; 
    pthread_mutex_lock(&program->print_lock);
    timestamp = get_time_ms() - program->start_time;
    printf("%ld %d burned out\n", timestamp, coder_id);
    pthread_mutex_unlock(&program->print_lock);
}
