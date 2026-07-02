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
#include <pthread.h>

void    compile(t_coder *coder)
{
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

void	*code_routine(void *arg)
{
    bool flag = true;
    while (flag)
    {
        t_coder *coder = (t_coder*)arg;
        simulation(coder);
        pthread_mutex_lock(&coder->program->my_mutex);
        compile(coder);
        pthread_mutex_unlock(&coder->program->my_mutex);
        debug(coder);
        refactore(coder);
    }
    return (NULL);
}

int	setup_coders(t_program program)
{
	int			i;
	int			status;
	pthread_t	t[program.data.number_of_coders];

	i = 0;
	while (i < program.data.number_of_coders)
	{
		program.coders[i].id = i;
		status = pthread_create(&t[i], NULL, code_routine, &program.coders[i]);
		if (status)
		{
			fprintf(stderr, "Thread %d creation failed with code %d", i,
				status);
			return (i);
		}
		program.coders[i].thread = t[i];
		i++;
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
