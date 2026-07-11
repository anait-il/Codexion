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

int	main(int ac, char *av[])
{
	int			state;
	t_program	program;

	state = parsing(ac, av, &program.data);
	if (state == 1)
		return (1);
	program.coders = malloc(sizeof(t_coder) * program.data.number_of_coders);
	if (!program.coders)
		return (1);
    pthread_mutex_init(&program.my_mutex, NULL);
    pthread_mutex_init(&program.print_lock, NULL);
    state = setup_dongles(&program);
    if (!state)
    {
        clean_up(program);
        return (1);
    }
	state = setup_coders(program);
	if (state)
	{
		clean_threads(program, state);
		return (1);
	}
	state = join_coders(program);
	if (state)
	{
		clean_up(program);
		return (1);
	}
    return (0);
}
