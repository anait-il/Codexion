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
	int	i;

	i = 0;
	program->dongles = malloc(sizeof(t_dongle)
			* program->data.number_of_coders);
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
	if (!is_running(coder->program))
		return (1);
	now = get_time_ms();
	if (now - dongle->release_time < coder->program->data.dongle_cooldown)
		return (0);
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

static int	acquire_first(t_dongle *dongle, t_coder *coder)
{
	int				state;
	t_coder			*status;
	long			wake_up;
	struct timespec	ts;

	pthread_mutex_lock(&dongle->lock);
	while (!can_take(dongle, coder))
	{
		wake_up = dongle->release_time + coder->program->data.dongle_cooldown
			+ 1;
        ts.tv_sec = wake_up / 1000;
        ts.tv_nsec = (wake_up % 1000) * 1000000L;
        pthread_cond_timedwait(&dongle->cond, &dongle->lock, &ts);
	}
	return (0);
}

static int	acquire_second(t_dongle *first, t_dongle *dongle, t_coder *coder)
{
	int				state;
	t_coder			*status;
	long			wake_up;

	pthread_mutex_lock(&dongle->lock);
	if (!can_take(dongle, coder))
	{
        pthread_mutex_unlock(&first->lock);
		pthread_mutex_unlock(&dongle->lock);
		return (1);
	}
	return (0);
}

int	acquire_dongles(t_coder *coder)
{
	int				state;
	t_dongle		*first;
	t_dongle		*second;
	struct timespec	ts;
	long			wake_up;

	first = assign_first(coder);
	second = assign_second(coder);
    pthread_mutex_lock(&coder->program->monitor_lock);
	coder->arrival_time = get_time_ms();
	coder->deadline = coder->last_compile_time
		+ coder->program->data.time_to_burnout;
    pthread_mutex_unlock(&coder->program->monitor_lock);
    pthread_mutex_lock(&first->lock);
	state = heap_push(&first->heap, coder);
    pthread_mutex_unlock(&first->lock);
	if (state)
	{
		return (1);
	}
    pthread_mutex_lock(&second->lock);
	state = heap_push(&second->heap, coder);
    pthread_mutex_unlock(&second->lock);
	if (state)
	{
		return (1);
	}
	while (is_running(coder->program))
	{
		if (!acquire_first(first, coder))
		{
			if (!acquire_second(first, second, coder))
				break;
            usleep(500);
			//wake_up = get_time_ms() - (second->relkease_time + coder->program->data.dongle_cooldown
			//	+ 1);
        	//ts.tv_sec = wake_up / 1000;
        	//ts.tv_nsec = (wake_up % 1000) * 1000000L;
		}
	}
    if (!is_running(coder->program))
        return (0);
	log_state(heap_pop(&first->heap), "has taken a dongle\n");
	log_state(heap_pop(&second->heap), "has taken a dongle\n");
	return (0);
}

void	release_dongles(t_coder *coder)
{
	if (!coder)
		return ;
	coder->left->release_time = get_time_ms();
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
