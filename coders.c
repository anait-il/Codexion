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
	pthread_mutex_lock(&coder->program->monitor_lock);
    coder->compile_counter++;
    coder->last_compile_time = get_time_ms();
	pthread_mutex_unlock(&coder->program->monitor_lock);
    log_state(coder, "is compiling\n");
    my_sleep(coder->program->data.time_to_compile, coder->program);
	// usleep(coder->program->data.time_to_compile * 1000);
}

void    debug(t_coder *coder)
{
    log_state(coder, "is debugging\n");
    my_sleep(coder->program->data.time_to_debug, coder->program);
	// usleep(coder->program->data.time_to_debug * 1000);
}

void    refactore(t_coder *coder)
{
    log_state(coder, "is refactoring\n");
    my_sleep(coder->program->data.time_to_refactor, coder->program);
	// usleep(coder->program->data.time_to_refactor * 1000);
}

int all_thread_ready(t_program *program)
{
	return (is_running(program));
}

bool    is_running(t_program *program)
{
    bool    status;

    pthread_mutex_lock(&program->monitor_lock);
    status = program->running;
    pthread_mutex_unlock(&program->monitor_lock);
    return (status);
}

void	run_even_only(t_coder *coder)
{
	if (coder->id % 2 != 0)
	{
		my_sleep(coder->program->data.time_to_compile, coder->program);
	}
}

void	*coder_routine(void *arg)
{
	bool	running;
	t_coder *coder;
	int		state;

	coder = (t_coder*)arg;
    pthread_mutex_lock(&coder->program->monitor_lock);
    while (true)
    {
        printf("coder %d, time %ld\n", coder->id, get_elapsed_ms(coder->program->start_time));
        pthread_cond_wait(&coder->program->barrier_cond, &coder->program->monitor_lock);
		if (coder->program->running)
			break;
    }
    pthread_mutex_unlock(&coder->program->monitor_lock);
	if (!is_running(coder->program))
		return (NULL);
	run_even_only(coder);
    while (is_running(coder->program))
	{
		state = acquire_dongles(coder);
		if (state)
			return (NULL);
        if (!is_running(coder->program))
        {
            release_dongles(coder);
            break;
        }
		if (is_running(coder->program))
        {
			compile(coder);
		    release_dongles(coder);
        }
		if (is_running(coder->program))
			debug(coder);
		if (is_running(coder->program))
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
		return (1);
	return (0);
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
			fprintf(stderr, "Thread %d join failed with code %d\n", y, status);
			return (1);
		}
		y++;
	}
	return (0);
}
