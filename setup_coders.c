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

int    setup_dongles(t_program *program)
{
    int     i;
    t_args  arguments;

    i = 0; 
    program->dongles = malloc(sizeof(t_dongle) * arguments.number_of_coders);
    if (!program->dongles)
        return (1);
    arguments = program->data;
    while (i < arguments.number_of_coders)
    {
        program->dongles[i].id = i;
        program->dongles[i].inuse = false;
        program->dongles[i].userid = 0;
        i++;
    }
    return (0);
}

void    acquire_dongle(t_coder *coder, t_dongle *dongle)
{
    pthread_mutex_t mutex;

    mutex = coder->program->my_mutex;
    pthread_mutex_lock(&mutex);

    //add coder to wait queue;
}

void	*code_routine(void *arg)
{
    t_coder *coder;
    bool    flag;

    flag = true;
    coder = (t_coder*)arg;
    if (!all_thread_ready(*coder))
        
    while (flag)
    {
        //acquire_dongle(coder);
        compile(coder);
        debug(coder);
        refactore(coder);
    }
    return(NULL);
}

void    assign_dongles(t_coder *coder, t_program *program, int counter)
{
    coder->left = &program->dongles[counter - 1];
    coder->right = &program->dongles[counter % program->data.number_of_coders];
}

int	setup_coders(t_program program)
{
	int			i;
	int			status;
	pthread_t	t[program.data.number_of_coders];

	i = 0;
	while (++i < program.data.number_of_coders)
	{
		program.coders[i].id = i;
        assign_dongles(&program.coders[i], &program, i);
		status = pthread_create(&t[i], NULL, code_routine, &program.coders[i]);
		if (status)
		{
			fprintf(stderr, "Thread %d creation failed with code %d", i,
				status);
			return (i);
		}
		program.coders[i].thread = t[i];
        pthread_cond_init(&program.coders[i].sleep, NULL);
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
