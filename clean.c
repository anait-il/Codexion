/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */ /*                                                    +:+ +:+         +:+     */
/*   By: anait-il <your@mail.com>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 15:55:18 by anait-il          #+#    #+#             */
/*   Updated: 2026/06/30 15:55:18 by anait-il         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "codexion.h"
#include <pthread.h>

void free_dongles(t_program *program)
{
    int i;

    i = 0;
    while (i < program->data.number_of_coders)
    {
        free(program->dongles[i].heap.arr);
        i++;
    }
    free(program->dongles);
}

void    destroy_mtx_cond(t_program *program)
{
    int i;

    i = 0;
    pthread_mutex_destroy(&program->print_lock);
    pthread_mutex_destroy(&program->monitor_lock);
    while (i < program->data.number_of_coders)
    {
        pthread_mutex_destroy(&program->dongles[i].lock);
        pthread_cond_destroy(&program->dongles[i].cond);
		i++;
    }
}

int	clean_up(t_program *program)
{
	int	i;

	i = 0;
    free_dongles(program);
	free(program->coders);
	return (0);
}

int	clean_threads(t_program *program, int coders_counter)
{
	int	i;
	int	status;

	i = 0;
	while (i < coders_counter)
	{
		status = pthread_join(program->coders[i].thread, NULL);
		if (status)
		{
			fprintf(stderr, "Thread %d join failed with code %d", i, status);
			break;
		}
		i++;
	}
	clean_up(program);
	return (0);
}
