/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anait-il <anait-il@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/31 10:43:00 by anait-il          #+#    #+#             */
/*   Updated: 2026/08/05 00:12:51 by anait-il         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	swap(t_coder **c1, t_coder **c2)
{
	t_coder	*tmp;

	tmp = *c1;
	*c1 = *c2;
	*c2 = tmp;
}

static int	tie_breaker(t_coder *a, t_coder *b)
{
	if (a->compile_counter != b->compile_counter)
		return (a->compile_counter < b->compile_counter);
	else
		return (a->id < b->id);
}

static int	has_priority(t_coder *a, t_coder *b, char *schedular)
{
	if (strcmp(schedular, "edf") == 0)
	{
		if (a->deadline - b->deadline != 0)
			return (a->deadline < b->deadline);
		return (tie_breaker(a, b));
	}
	else
	{
		if (a->arrival_time - b->arrival_time != 0)
			return (a->arrival_time > b->arrival_time);
		return (tie_breaker(a, b));
	}
}

void	bubble_up(t_heap *heap, char *schedular)
{
	int	i;
	int	parent;

	i = heap->size - 1;
	parent = (i - 1) / 2;
	while (i > 0 && has_priority(heap->arr[i], heap->arr[parent], schedular))
	{
		swap(&heap->arr[i], &heap->arr[parent]);
		i = parent;
		parent = (i - 1) / 2;
	}
}

void	bubble_down(t_heap *heap, char *schedular)
{
	int	i;
	int	left_child;
	int	right_child;
	int	smallest;

	i = 0;
	left_child = (i * 2) + 1;
	right_child = (i * 2) + 2;
	while (left_child < heap->size)
	{
		smallest = left_child;
		if (right_child < heap->size && !has_priority(heap->arr[left_child],
				heap->arr[right_child], schedular))
			smallest = right_child;
		if (has_priority(heap->arr[i], heap->arr[smallest], schedular))
			break ;
		swap(&heap->arr[i], &heap->arr[smallest]);
		i = smallest;
		left_child = (i * 2) + 1;
		right_child = (i * 2) + 2;
	}
}
