/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdelkabir <abdelkabir@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 12:12:05 by anait-il          #+#    #+#             */
/*   Updated: 2026/07/30 06:33:40 by abdelkabir       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	assign_dongles(t_coder *coder, t_program *program, int counter)
{
	coder->left = &program->dongles[counter - 1];
	coder->right = &program->dongles[counter % program->data.number_of_coders];
}

static void	init_dongle_param(t_dongle *dongle, int id, t_program *program)
{
	pthread_mutex_init(&dongle->lock, NULL);
	pthread_cond_init(&dongle->cond, NULL);
	dongle->id = id;
	dongle->release_time = 0;
	dongle->available = true;
	dongle->heap.size = 0;
	dongle->heap.program = program;
	dongle->heap.capacity = program->data.number_of_coders;
}

int	setup_dongles(t_program *program)
{
	int	i;

	i = 0;
	program->dongles = malloc(sizeof(t_dongle)
			* program->data.number_of_coders);
	if (!program->dongles)
		return (1);
	while (i < program->data.number_of_coders)
	{
		init_dongle_param(&program->dongles[i], i, program);
		program->dongles[i].heap.arr = malloc(sizeof(t_coder *)
				* program->data.number_of_coders);
		if (!program->dongles[i].heap.arr)
		{
			free_dongles(program, i);
			return (1);
		}
		i++;
	}
	return (0);
}
