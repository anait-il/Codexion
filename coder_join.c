/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_join.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anait-il <anait-il@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 18:34:57 by anait-il          #+#    #+#             */
/*   Updated: 2026/08/01 15:59:19 by anait-il         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	wait_for_start(t_coder *coder)
{
	pthread_mutex_lock(&coder->program->monitor_lock);
	while (!coder->program->started)
		pthread_cond_wait(&coder->program->barrier_cond,
			&coder->program->monitor_lock);
	coder->last_compile_time = get_time_ms();
	coder->program->ready_count++;
	if (coder->program->ready_count == coder->program->data.number_of_coders)
		pthread_cond_signal(&coder->program->ready_cond);
	pthread_mutex_unlock(&coder->program->monitor_lock);
}

int	join_coders(t_program *program)
{
	int	y;
	int	status;

	y = 0;
	while (y < program->data.number_of_coders)
	{
		status = pthread_join(program->coders[y].thread, NULL);
		if (status)
		{
			fprintf(stderr, "Error: Thread %d join failed with code %d\n", y,
				status);
			return (1);
		}
		y++;
	}
	return (0);
}
