/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_logic.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anait-il <anait-il@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 12:29:37 by anait-il          #+#    #+#             */
/*   Updated: 2026/08/02 21:06:50 by anait-il         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	acquire_first(t_dongle *first, t_coder *coder)
{
	int				state;
	t_coder			*status;
	long			wake_up;
	struct timespec	ts;

	pthread_mutex_lock(&first->lock);
	while (!can_take(first, coder))
	{
		wake_up = first->release_time + coder->program->data.dongle_cooldown;
		ts.tv_sec = wake_up / 1000;
		ts.tv_nsec = (wake_up % 1000) * 1000000L;
		pthread_cond_timedwait(&first->cond, &first->lock, &ts);
	}
	return (0);
}

static int	acquire_second(t_dongle *first, t_dongle *second, t_coder *coder)
{
	int		state;
	t_coder	*status;
	long	wake_up;

	pthread_mutex_lock(&second->lock);
	if (!can_take(second, coder))
	{
		pthread_mutex_unlock(&second->lock);
		pthread_mutex_unlock(&first->lock);
		return (1);
	}
	first->available = false;
	second->available = false;
	pthread_mutex_unlock(&second->lock);
	pthread_mutex_unlock(&first->lock);
	return (0);
}

void	setup_schedular_times(t_coder *coder)
{
	pthread_mutex_lock(&coder->program->monitor_lock);
	coder->arrival_time = get_time_ms();
	coder->deadline = coder->last_compile_time
		+ coder->program->data.time_to_burnout;
	pthread_mutex_unlock(&coder->program->monitor_lock);
}

static int	request_dongles(t_coder *coder, t_dongle *first, t_dongle *second)
{
	int	state;

	setup_schedular_times(coder);
	pthread_mutex_lock(&first->lock);
	state = heap_push(&first->heap, coder);
	pthread_mutex_unlock(&first->lock);
	if (state)
		return (1);
	pthread_mutex_lock(&second->lock);
	state = heap_push(&second->heap, coder);
	pthread_mutex_unlock(&second->lock);
	if (state)
		return (1);
	return (0);
}

int	acquire_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	assign_order(coder, &first, &second);
	if (request_dongles(coder, first, second))
		return (1);
	if (first == second)
	{
		acquire_first(first, coder);
        pthread_mutex_unlock(&first->lock);
		log_state(heap_pop(&first->heap), "has taken a dongle\n");
		return (1);
	}
	while (is_running(coder->program))
	{
		acquire_first(first, coder);
		if (!acquire_second(first, second, coder))
			break ;
		my_sleep(1, coder->program);
	}
	if (!is_running(coder->program))
		return (1);
	pop(coder, first, second);
	log_state(coder, "has taken a dongle\n");
	log_state(coder, "has taken a dongle\n");
	return (0);
}
