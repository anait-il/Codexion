/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anait-il <your@mail.com>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 15:54:59 by anait-il          #+#    #+#             */
/*   Updated: 2026/07/28 16:44:25 by anait-il         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <pthread.h>

static int init_program(t_program *program)
{
    int status;

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
        clean_threads(&program, state);
        return (8);
    }
    if (join_coders(&program))
    {
        clean_up(&program);
        return (1);
    }
    if (pthread_join(program.monitor, NULL))
    {
        clean_up(&program);
        destroy_mtx_cond(&program);
        return (1);
    }
    destroy_mtx_cond(&program);
    clean_up(&program);
    return (0);
}
