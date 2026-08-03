/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anait-il <anait-il@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 18:55:57 by anait-il          #+#    #+#             */
/*   Updated: 2026/08/03 19:17:05 by anait-il         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	can_take(t_dongle *dongle, t_coder *coder)
{
	long	now;
	t_coder	*top;

	if (!is_running(coder->program))
		return (1);
	if (!dongle->available)
		return (0);
	now = get_time_ms();
	if (now - dongle->release_time < coder->program->data.dongle_cooldown)
		return (0);
	if (heap_top(&dongle->heap) != coder)
		return (0);
	return (1);
}

void	assign_order(t_coder *coder, t_dongle **first, t_dongle **second)
{
	if (coder->id % 2 == 0)
	{
		*first = coder->left;
		*second = coder->right;
	}
	else
	{
		*first = coder->right;
		*second = coder->left;
	}
}

void	release_dongles(t_coder *coder)
{
	if (!coder)
		return ;
	pthread_mutex_lock(&coder->left->lock);
	coder->left->release_time = get_time_ms();
	coder->left->available = true;
	pthread_cond_broadcast(&coder->left->cond);
	pthread_mutex_unlock(&coder->left->lock);
	pthread_mutex_lock(&coder->right->lock);
	coder->right->release_time = get_time_ms();
	coder->right->available = true;
	pthread_cond_broadcast(&coder->right->cond);
	pthread_mutex_unlock(&coder->right->lock);
}

void	pop(t_coder *coder, t_dongle *first, t_dongle *second)
{
	pthread_mutex_lock(&first->lock);
	heap_pop(&first->heap);
	pthread_mutex_unlock(&first->lock);
	pthread_mutex_lock(&second->lock);
	heap_pop(&second->heap);
	pthread_mutex_unlock(&second->lock);
}

void	only_one_coder(t_dongle *dongle, t_coder *coder)
{
	acquire_first(dongle, coder);
	pthread_mutex_unlock(&dongle->lock);
	log_state(heap_pop(&dongle->heap), "has taken a dongle\n");
}
