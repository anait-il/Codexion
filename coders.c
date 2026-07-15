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

void    compile(t_coder *coder)
{
    pthread_t           monitor;

    t_program *program = coder->program;
    printf("%d is compiling", coder->id);
    usleep(program->data.time_to_compile);
}

void    debug(t_coder *coder)
{
    t_program *program = coder->program;
    printf("%d is debugging", coder->id);
    usleep(program->data.time_to_debug);
}

void    refactore(t_coder *coder)
{
    t_program *program = coder->program;
    printf("%d is refactoring", coder->id);
    usleep(program->data.time_to_refactor);
}

int all_thread_ready(t_program program)
{
    if (program.number_of_coders == program.data.number_of_coders)
        return (1);
    return (0);
}

void	*coder_routine(void *arg)
{
    t_coder *coder;

    coder = (t_coder*)arg;
    while (!all_thread_ready(*coder->program))
        usleep(1000);
    acquire_dongles(coder);
    compile(coder);
    debug(coder);
    
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
	while (++i < program->data.number_of_coders)
	{
		program->coders[i].id = i;
        program->number_of_coders = i;
        assign_dongles(&program->coders[i], program, i);
		status = pthread_create(&t[i], NULL, coder_routine, &program->coders[i]);
		if (status)
		{
			fprintf(stderr, "Thread %d creation failed with code %d", i,
				status);
			return (i);
		}
		program->coders[i].thread = t[i];
        pthread_cond_init(&program->coders[i].sleep, NULL);
        program->coders[i].compile_counter = 0;
        program->coders[i].last_compile_time = 0;
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
