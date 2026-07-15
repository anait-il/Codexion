/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anait-il <your@mail.com>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 12:12:05 by anait-il          #+#    #+#             */
/*   Updated: 2026/07/13 12:42:44 by anait-il         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	setup_dongles(t_program *program)
{
	int		i;
	t_args	arguments;

	i = 0;
	arguments = program->data;
	program->dongles = malloc(sizeof(t_dongle) * arguments.number_of_coders);
	if (!program->dongles)
		return (1);
	while (i < arguments.number_of_coders)
	{
		pthread_mutex_init(&program->dongles[i].lock, NULL);
		pthread_cond_init(&program->dongles[i].cond, NULL);
		program->dongles[i].id = i;
		program->dongles[i].release_time = 0;
		program->dongles[i].available = true;
		program->dongles[i].heap.size = 0;
		program->dongles[i].heap.program = program;
		program->dongles[i].heap.capacity = arguments.number_of_coders;
		program->dongles[i].heap.arr = malloc(sizeof(t_coder *)
				* arguments.number_of_coders);
		if (!program->dongles[i].heap.arr)
			return (1);
		i++;
	}
	return (0);
}

int	can_take(t_dongle *dongle, t_coder *coder)
{
	long	now;

	if (!dongle || !coder)
		return (-1);
	if (!dongle->available)
		return (0);
	now = get_time_ms();
	if (now - dongle->release_time < coder->program->data.dongle_cooldown)
		return (0);
	if (heap_top(&dongle->heap) != coder)
		return (0);
	return (1);
}

t_dongle	*assign_first(t_coder *coder)
{
	if (coder->id % 2 == 0)
		return (coder->left);
	return (coder->right);
}

t_dongle	*assign_second(t_coder *coder)
{
	if (coder->id % 2 == 0)
		return (coder->right);
	return (coder->left);
}

int	acquire_one(t_dongle *dongle, t_coder *coder)
{
	int		state;
	t_coder	*status;

	pthread_mutex_lock(&dongle->lock);
    coder->arrival_time = get_time_ms();
    coder->deadline = coder->last_compile_time + coder->program->data.time_to_burnout;
	state = heap_push(&dongle->heap, coder);
	if (state)
		return (1);
	while (!can_take(dongle, coder))
		pthread_cond_wait(&dongle->cond, &dongle->lock);
    return (0);
}

int	acquire_dongles(t_coder *coder)
{
	int			state;
	t_dongle	*first;
	t_dongle	*second;

	first = assign_first(coder);
	second = assign_second(coder);
	state = acquire_one(first, coder);
	if (state)
		return (1);
	state = acquire_one(second, coder);
	if (state)
		return (1);
    heap_pop(&first->heap);
    heap_pop(&second->heap);
    first->available = false;
    second->available = false;
	return (0);
}

void	release_dongles(t_coder *coder)
{
	if (!coder)
		return ;
	coder->left->available = true;
	coder->left->release_time = get_time_ms();
	coder->right->available = true;
	coder->right->release_time = get_time_ms();
    pthread_cond_broadcast(&coder->left->cond);
    pthread_cond_broadcast(&coder->right->cond);
    pthread_mutex_unlock(&coder->left->lock);
    pthread_mutex_unlock(&coder->right->lock);
}

void	assign_dongles(t_coder *coder, t_program *program, int counter)
{
	coder->left = &program->dongles[counter - 1];
	coder->right = &program->dongles[counter % program->data.number_of_coders];
}
