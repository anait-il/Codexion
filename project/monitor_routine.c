/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_routine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anait-il <anait-il@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/31 10:50:19 by anait-il          #+#    #+#             */
/*   Updated: 2026/08/06 15:33:49 by anait-il         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	detect_end_compile(t_program *program)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&program->monitor_lock);
	while (i < program->data.number_of_coders)
	{
		if (program->coders[i].compile_counter < \
				program->data.number_of_compiles_required)
		{
			pthread_mutex_unlock(&program->monitor_lock);
			return (1);
		}
		i++;
	}
	program->running = false;
	pthread_mutex_unlock(&program->monitor_lock);
	return (0);
}

static int	detect_burnout(t_program *program)
{
	int		i;
	long	now;

	i = 0;
	pthread_mutex_lock(&program->monitor_lock);
	now = get_elapsed_ms(program->start_time);
	while (i < program->data.number_of_coders)
	{
		if ((now >= program->coders[i].last_compile_time + \
			program->data.time_to_burnout))
		{
			program->running = false;
			pthread_mutex_unlock(&program->monitor_lock);
			return (i + 1);
		}
		i++;
	}
	pthread_mutex_unlock(&program->monitor_lock);
	return (0);
}

static void	run_simulation(t_program *program)
{
	pthread_mutex_lock(&program->monitor_lock);
	program->running = true;
	program->started = true;
	program->start_time = get_time_ms();
	pthread_cond_broadcast(&program->barrier_cond);
	pthread_mutex_unlock(&program->monitor_lock);
}

void	*monitor_routine(void *arg)
{
	t_program	*program;
	int			state;

	program = (t_program *)arg;
	run_simulation(program);
	while (true)
	{
		usleep(100);
		state = detect_burnout(program);
		if (state)
		{
			log_burnout(program, state);
			break ;
		}
		if (!detect_end_compile(program))
			break ;
	}
	stop_simulation(program);
	return (NULL);
}
