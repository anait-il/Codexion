/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_setup.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anait-il <anait-il@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/27 01:36:10 by anait-il          #+#    #+#             */
/*   Updated: 2026/08/05 23:57:22 by anait-il         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool	is_running(t_program *program)
{
	bool	status;

	pthread_mutex_lock(&program->monitor_lock);
	status = program->running;
	pthread_mutex_unlock(&program->monitor_lock);
	return (status);
}

void	run_even_only(t_coder *coder)
{
	long	sleep_time;

	sleep_time = (coder->program->data.time_to_compile
			+ coder->program->data.dongle_cooldown) / 2;
	if (coder->id % 2 != 0)
		my_sleep(sleep_time, coder->program);
}

static void	init_coder_param(t_coder *coder, t_program *program, int i)
{
	coder->id = i + 1;
	assign_dongles(coder, program, i + 1);
	coder->program = program;
	coder->last_compile_time = 0;
	coder->compile_counter = 0;
}

static int	program_cond_init(t_program *program)
{
	program->coders = malloc(sizeof(t_coder) * program->data.number_of_coders);
	if (!program->coders)
	{
		fprintf(stderr, "Error: coders allocation failed\n");
		dongles_destroy(program, program->data.number_of_coders);
		destroy_program_lock(program);
		free_dongles(program, program->data.number_of_coders);
		return (1);
	}
	if (pthread_cond_init(&program->barrier_cond, NULL))
	{
		dongles_destroy(program, program->data.number_of_coders);
		destroy_program_lock(program);
		free_dongles(program, program->data.number_of_coders);
		free(program->coders);
		return (1);
	}
	return (0);
}

int	setup_coders(t_program *program)
{
	int			i;
	int			status;

	i = 0;
	if (program_cond_init(program))
		return (-1);
	while (i < program->data.number_of_coders)
	{
		init_coder_param(&program->coders[i], program, i);
		status = pthread_create(&program->coders[i].thread,
				NULL, coder_routine, &program->coders[i]);
		if (status)
		{
			fprintf(stderr, "Error: Thread %d creation failed with code %d\n", i
				+ 1, status);
			dongles_destroy(program, program->data.number_of_coders);
			return (i);
		}
		i++;
	}
	if (start_monitoring(program))
		return (i);
	return (0);
}
