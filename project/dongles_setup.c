/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles_setup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anait-il <anait-il@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 12:12:05 by anait-il          #+#    #+#             */
/*   Updated: 2026/08/06 15:03:05 by anait-il         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	assign_dongles(t_coder *coder, t_program *program, int counter)
{
	coder->left = &program->dongles[counter - 1];
	coder->right = &program->dongles[counter % program->data.number_of_coders];
}

static int	init_dongle_param(t_dongle *dongle, int id, t_program *program)
{
	if (pthread_mutex_init(&dongle->lock, NULL))
	{
		destroy_program_lock(program);
		dongles_destroy(program, id);
		free_dongles(program, id);
		return (1);
	}
	if (pthread_cond_init(&dongle->cond, NULL))
	{
		destroy_program_lock(program);
		pthread_mutex_destroy(&dongle->lock);
		dongles_destroy(program, id);
		free_dongles(program, id);
		return (1);
	}
	dongle->id = id;
	dongle->release_time = 0;
	dongle->available = true;
	dongle->heap.size = 0;
	dongle->heap.program = program;
	dongle->heap.capacity = 2;
	return (0);
}

int	setup_dongles(t_program *program)
{
	int	i;

	i = 0;
	program->dongles = malloc(sizeof(t_dongle)
			* program->data.number_of_coders);
	if (!program->dongles)
	{
		destroy_program_lock(program);
		fprintf(stderr, "Error: dongle allocation failled\n");
		return (1);
	}
	while (i < program->data.number_of_coders)
	{
		if (init_dongle_param(&program->dongles[i], i, program))
			return (1);
		program->dongles[i].heap.arr = malloc(sizeof(t_coder *)
				* 2);
		if (!program->dongles[i].heap.arr)
		{
			free_dongles(program, i);
			return (1);
		}
		i++;
	}
	return (0);
}
