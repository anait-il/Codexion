/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdelkabir <abdelkabir@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 14:30:48 by anait-il          #+#    #+#             */
/*   Updated: 2026/08/02 12:27:19 by abdelkabir       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	start_monitoring(t_program *program)
{
	int	status;

	status = pthread_create(&program->monitor, NULL, monitor_routine, program);
	if (status)
	{
		fprintf(stderr, "Error: monitor create failed with code %d\n", status);
		free_dongles(program, program->data.number_of_coders);
		return (status);
	}
	return (0);
}

void	stop_simulation(t_program *program)
{
	int	i;

	i = 0;
	while (i < program->data.number_of_coders)
	{
		pthread_mutex_lock(&program->dongles[i].lock);
		pthread_cond_broadcast(&program->dongles[i].cond);
		pthread_mutex_unlock(&program->dongles[i].lock);
		i++;
	}
}
