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

	i = 0;
	program->dongles = malloc(sizeof(t_dongle) * program->data.number_of_coders);
	if (!program->dongles)
		return (1);
	while (i < program->data.number_of_coders)
	{
		pthread_mutex_init(&program->dongles[i].lock, NULL);
		pthread_cond_init(&program->dongles[i].cond, NULL);
		program->dongles[i].id = i;
		program->dongles[i].release_time = 0;
		program->dongles[i].available = true;
		program->dongles[i].heap.size = 0;
		program->dongles[i].heap.program = program;
		program->dongles[i].heap.capacity = program->data.number_of_coders;
		program->dongles[i].heap.arr = malloc(sizeof(t_coder *)
				* program->data.number_of_coders);
		if (!program->dongles[i].heap.arr)
        {
            free(program->dongles);
            return (1);
        }
		i++;
	}
	return (0);
}

static int	can_take(t_dongle *dongle, t_coder *coder)
{
	long	now;

	if (!dongle || !coder)
		return (-1);
	if (!dongle->available)
		return (0);
	now = get_time_ms();
	printf("[now: %ld, release_time: %ld < dongle_cooldown: %lld] = %ld\n", now,  dongle->release_time, coder->program->data.dongle_cooldown , now - dongle->release_time);
	if (now - dongle->release_time < coder->program->data.dongle_cooldown)
	{
		return (0);
	}
	if (heap_top(&dongle->heap) != coder)
		return (0);
	return (1);
}

static t_dongle	*assign_first(t_coder *coder)
{
	if (coder->id % 2 == 0)
		return (coder->left);
	return (coder->right);
}

static t_dongle	*assign_second(t_coder *coder)
{
	if (coder->id % 2 == 0)
		return (coder->right);
	return (coder->left);
}

static int	acquire_one(t_dongle *dongle, t_coder *coder)
{
	int		state;
	t_coder	*status;

	pthread_mutex_lock(&dongle->lock);
    coder->arrival_time = get_time_ms();
    coder->deadline = coder->last_compile_time + coder->program->data.time_to_burnout;
	state = heap_push(&dongle->heap, coder);
	if (state)
		return (1);
	printf("coder %d tries to lock dongle %d\n", coder->id, dongle->id);
	while (true)
	{
		int state = can_take(dongle, coder);
		printf("coder %d , dongle %d => state {%d}\n",coder->id , dongle->id, state);
		if (state)
			break;
		pthread_cond_wait(&dongle->cond, &dongle->lock);
	}
	printf("coder %d success to lock dongle %d\n", coder->id, dongle->id);
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
    log_state(heap_pop(&first->heap), "has taken a dongle\n");
    log_state(heap_pop(&second->heap), "has taken a dongle\n");
    first->available = false;
    second->available = false;
	return (0);
}

void	release_dongles(t_coder *coder)
{
	if (!coder)
		return ;
    coder->left->release_time = get_time_ms();
    coder->right->release_time = get_time_ms();
	coder->left->available = true;
	coder->left->release_time = get_time_ms();
	coder->right->available = true;
	coder->right->release_time = get_time_ms();
    pthread_mutex_unlock(&coder->left->lock);
    pthread_mutex_unlock(&coder->right->lock);
    pthread_cond_broadcast(&coder->left->cond);
    pthread_cond_broadcast(&coder->right->cond);
	printf("coder {%d} release dongle {%d, %d}\n", coder->id, coder->left->id,coder->right->id);
}

void	assign_dongles(t_coder *coder, t_program *program, int counter)
{
	coder->left = &program->dongles[counter - 1];
	coder->right = &program->dongles[counter % program->data.number_of_coders];
}
