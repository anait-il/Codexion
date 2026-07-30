/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anait-il <anait-il@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 15:54:59 by anait-il          #+#    #+#             */
/*   Updated: 2026/07/30 19:16:09 by anait-il         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <pthread.h>

static int	init_program(t_program *program)
{
	int	status;

	program->running = false;
	program->started = false;
	status = pthread_mutex_init(&program->monitor_lock, NULL);
	if (status)
		return (1);
	status = pthread_mutex_init(&program->print_lock, NULL);
	if (status)
		return (1);
	return (0);
}

static int	full_clean_exit(t_program *program, int status)
{
	destroy_mtx_cond(program);
	clean_up(program);
	return (status);
}

int	main(int ac, char *av[])
{
	int			state;
	t_program	program;

	state = parsing(ac, av, &program);
	if (state)
		return (1);
	if (init_program(&program))
		return (1);
	if (setup_dongles(&program))
		return (1);
	state = setup_coders(&program);
	if (state)
	{
		printf("helere\n");
		clean_threads(&program, state);
		return (8);
	}
	if (join_coders(&program))
	{
		clean_up(&program);
		return (1);
	}
	if (pthread_join(program.monitor, NULL))
		return (full_clean_exit(&program, 1));
	return (full_clean_exit(&program, 0));
}
