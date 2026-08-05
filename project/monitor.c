/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anait-il <anait-il@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 14:30:48 by anait-il          #+#    #+#             */
/*   Updated: 2026/08/05 18:37:11 by anait-il         ###   ########.fr       */
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
