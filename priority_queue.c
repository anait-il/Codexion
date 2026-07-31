/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   priority_queue.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anait-il <your@mail.com>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/31 10:40:22 by anait-il          #+#    #+#             */
/*   Updated: 2026/07/31 10:43:56 by anait-il         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	heap_push(t_heap *heap, t_coder *coder)
{
	t_coder	*arr;

	if (!heap || !coder || !heap->arr)
		return (1);
	if (heap->size >= heap->capacity)
	{
		fprintf(stderr, "Heap overflow\n");
		return (1);
	}
	bubble_up(heap, coder->program->data.scheduler);
	return (0);
}

t_coder	*heap_pop(t_heap *heap)
{
	t_coder	*tmp;
	int		i;

	if (!heap || !heap->arr)
		return (NULL);
	i = 0;
	tmp = heap->arr[i];
	heap->arr[i] = heap->arr[heap->size - 1];
	heap->size--;
	bubble_down(heap, heap->program->data.scheduler);
	return (tmp);
}

t_coder	*heap_top(t_heap *heap)
{
	if (!heap || heap->size == 0)
		return (NULL);
	return (heap->arr[0]);
}
