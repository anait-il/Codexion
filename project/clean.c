/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anait-il <anait-il@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 18:51:15 by anait-il          #+#    #+#             */
/*   Updated: 2026/08/06 15:32:54 by anait-il         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	free_dongles(t_program *program, int total)
{
	int	i;

	i = 0;
	while (i < total)
	{
		free(program->dongles[i].heap.arr);
		i++;
	}
	free(program->dongles);
}

void	destroy_mtx_cond(t_program *program)
{
	int	i;

	i = 0;
	pthread_mutex_destroy(&program->print_lock);
	pthread_mutex_destroy(&program->monitor_lock);
	pthread_cond_destroy(&program->barrier_cond);
	while (i < program->data.number_of_coders)
	{
		pthread_mutex_destroy(&program->dongles[i].lock);
		pthread_cond_destroy(&program->dongles[i].cond);
		i++;
	}
}

int	clean_up(t_program *program)
{
	free_dongles(program, program->data.number_of_coders);
	free(program->coders);
	return (0);
}

void	dongles_destroy(t_program *program, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pthread_mutex_destroy(&program->dongles[i].lock);
		pthread_cond_destroy(&program->dongles[i].cond);
		i++;
	}
}

int	clean_threads(t_program *program, int coders_counter)
{
	int	i;
	int	status;

	i = 0;
	if (coders_counter == -1)
		return (0);
	pthread_mutex_lock(&program->monitor_lock);
	program->started = true;
	pthread_cond_broadcast(&program->barrier_cond);
	pthread_mutex_unlock(&program->monitor_lock);
	while (i < coders_counter)
	{
		status = pthread_join(program->coders[i].thread, NULL);
		if (status)
		{
			fprintf(stderr,
				"Thread %d join failed with code %d", i + 1, status);
			break ;
		}
		i++;
	}
	destroy_program_lock(program);
	pthread_cond_destroy(&program->barrier_cond);
	clean_up(program);
	return (0);
}
