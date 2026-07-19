/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anait-il <your@mail.com>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/27 01:36:10 by anait-il          #+#    #+#             */
/*   Updated: 2026/07/17 14:51:02 by anait-il         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

pthread_mutex_t mutex;

void    compile(t_coder *coder)
{
    coder->compile_counter++;
    coder->last_compile_time = get_time_ms();
    log_state(coder, "is compiling\n");
    usleep(coder->program->data.time_to_compile);
}

void    debug(t_coder *coder)
{
    log_state(coder, "is debugging\n");
    usleep(coder->program->data.time_to_debug);
}

void    refactore(t_coder *coder)
{
    log_state(coder, "is refactoring\n");
    usleep(coder->program->data.time_to_refactor);
}

int all_thread_ready(t_program *program)
{
	bool	running;

	pthread_mutex_lock(&program->monitor_lock);
	running = program->running;
	pthread_mutex_unlock(&program->monitor_lock);
	if (running)
		return (1);
	return (0);
}

void	*coder_routine(void *arg)
{
	bool	running;
	t_coder *coder;
	int		state;

 	coder = (t_coder*)arg;
    while (!all_thread_ready(coder->program))
        usleep(100);
	while (coder->program->running)
	{
		state = acquire_dongles(coder);
		if (state)
		{	
			return (NULL);
		}
		if (coder->program->running)
			compile(coder);
			release_dongles(coder);
		if (coder->program->running)
			debug(coder);
		if (coder->program->running)
			refactore(coder);
	}
    return (NULL);
}

int	setup_coders(t_program *program)
{
	int			i;
	int			status;
	pthread_t	t[program->data.number_of_coders];

	i = 0;
    program->coders = malloc(sizeof(t_coder) * program->data.number_of_coders);
	if (!program->coders)
	    return (1);
	program->start_time = get_time_ms();
	while (i < program->data.number_of_coders)
	{
		program->coders[i].id = i + 1;
        assign_dongles(&program->coders[i], program, i+1);
		program->coders[i].program = program;
		program->coders[i].last_compile_time = program->start_time;
		program->coders[i].compile_counter = 0;
		status = pthread_create(&t[i], NULL, coder_routine, &program->coders[i]);
		if (status)
		{
			fprintf(stderr, "Thread %d creation failed with code %d\n", i + 1,
				status);
			return (i);
		}
		program->coders[i].thread = t[i];
		i++;
    }
    status = start_monitoring(program);
	if (status)
	{
		fprintf(stderr, "Error: monitor creation failed with code %d\n", status);
		return (1);
	}
	return (0);
}

int	join_coders(t_program program)
{
	int	y;
	int	status;

	y = 0;
	while (y < program.data.number_of_coders)
	{
		status = pthread_join(program.coders[y].thread, NULL);	
		if (status)
		{
			fprintf(stderr, "Thread %d join failed with code %d\n", y, status);
			return (1);
		}
		y++;
	}
	return (0);
}
