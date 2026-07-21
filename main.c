/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anait-il <your@mail.com>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 15:54:59 by anait-il          #+#    #+#             */
/*   Updated: 2026/06/30 15:54:59 by anait-il         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <pthread.h>

int init_mutex(t_program *program)
{
    int status;

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

    program.running = false;
	state = parsing(ac, av, &program);
	if (state == 1)
		return (1);
    state = init_mutex(&program);
    state = setup_dongles(&program);
    if (state)
        return (7);
	state = setup_coders(&program);
	if (state)
	{
		clean_threads(&program, state);
		return (8);
	}
    state = join_coders(&program);
	if (state)
	{
        clean_up(&program);
		return (1);
	}
    state = pthread_join(program.monitor, NULL);
    if (state)
    {
        clean_up(&program);
        destroy_mtx_cond(&program);
        return (1);
    }
    destroy_mtx_cond(&program);
    clean_up(&program);
    printf("########### program is finish ##############\n");
    return (0);
}
