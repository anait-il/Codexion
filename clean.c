/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */ /*                                                    +:+ +:+         +:+     */
/*   By: anait-il <your@mail.com>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 15:55:18 by anait-il          #+#    #+#             */
/*   Updated: 2026/06/30 15:55:18 by anait-il         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "codexion.h"

int	clean_up(t_program *program)
{
	int	i;

	i = 0;
	free(program->coders);
	return (0);
}

int	clean_threads(t_program *program, int coders_counter)
{
	int	i;
	int	status;

	i = 0;
	while (i < coders_counter)
	{
		status = pthread_join(program->coders[i].thread, NULL);
		if (status)
		{
			fprintf(stderr, "Thread %d join failed with code %d", i, status);
			break;
		}
		i++;
	}
	clean_up(program);
	return (0);
}
