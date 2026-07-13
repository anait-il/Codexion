/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anait-il <your@mail.com>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 12:12:05 by anait-il          #+#    #+#             */
/*   Updated: 2026/07/13 12:42:44 by anait-il         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int    setup_dongles(t_program *program)
{
    int     i;
    t_args  arguments;

    i = 0; 
    arguments = program->data;
    program->dongles = malloc(sizeof(t_dongle) * arguments.number_of_coders);
    if (!program->dongles)
        return (1);
    while (i < arguments.number_of_coders)
    {
        pthread_mutex_init(&program->dongles[i].lock, NULL);
        pthread_cond_init(&program->dongles[i].cond, NULL);
        program->dongles[i].id = i;
        program->dongles[i].release_time = 0;
        program->dongles[i].available = true;
        program->dongles[i].heap.size = 0;
        program->dongles[i].heap.program = program;
        program->dongles[i].heap.capacity = arguments.number_of_coders;
        program->dongles[i].heap.arr = malloc(sizeof(t_coder *) * arguments.number_of_coders);
        if (!program->dongles[i].heap.arr)
            return (1);
        i++;
    }
    return (0);
}

int can_take(t_dongle *dongle, t_coder *coder)
{
    long    now;
    if (!dongle || !coder)
        return (-1);

    if (!dongle->available)
        return (0);
    now = get_time_ms();
    if (now - dongle->release_time < coder->program->data.dongle_cooldown)
        return (0);
    if (heap_top(&dongle->heap) != coder)
        return (0);
    return (1);
}

void    acquire_dongle(t_coder *coder, t_dongle *dongle)
{
    int state;

    pthread_mutex_lock(&dongle->lock);
    state = heap_push(&dongle->heap, coder);

    //add coder to wait queue;

}

void    assign_dongles(t_coder *coder, t_program *program, int counter)
{
    coder->left = &program->dongles[counter - 1];
    coder->right = &program->dongles[counter % program->data.number_of_coders];
}
