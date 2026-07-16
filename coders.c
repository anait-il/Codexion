/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_coders.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anait-il <your@mail.com>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/27 01:36:10 by anait-il          #+#    #+#             */
/*   Updated: 2026/06/27 01:56:38 by anait-il         ###   ########.fr       */
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

int all_thread_ready(t_coder *coder)
{
	if (coder->id == coder->program->data.number_of_coders)
		return (1);
	return (0);
}

void	*coder_routine(void *arg)
{
    t_coder *coder;

 	coder = (t_coder*)arg;
    // while (!all_thread_ready(coder))
    //     usleep(100);
    acquire_dongles(coder);
    compile(coder);
	release_dongles(coder);
    debug(coder);
    refactore(coder);
    return (NULL);
}     

int	setup_coders(t_program *program)
{
	int			i;
	int			status;
	pthread_t	t[program->data.number_of_coders];

	i = 0;
	program->start_time = get_time_ms();
	program->running = 1;
    program->coders = malloc(sizeof(t_coder) * program->data.number_of_coders);
	if (!program->coders)
		return (1);
	while (++i < program->data.number_of_coders)
	{
		program->coders[i - 1].id = i;
        assign_dongles(&program->coders[i - 1], program, i);
		program->coders[i - 1].program = program;
		status = pthread_create(&t[i - 1], NULL, coder_routine, &program->coders[i - 1]);
		if (status)
		{
			fprintf(stderr, "Thread %d creation failed with code %d", i,
				status);
			return (i);
		}
		program->coders[i - 1].thread = t[i - 1];
        pthread_cond_init(&program->coders[i - 1].sleep, NULL);
        program->coders[i - 1].compile_counter = 0;
        program->coders[i - 1].last_compile_time = 0;
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
			fprintf(stderr, "Thread %d join failed with code %d", y, status);
			return (1);
		}
		y++;
	}
	return (0);
}
