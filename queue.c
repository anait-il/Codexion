/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anait-il <your@mail.com>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 16:01:24 by anait-il          #+#    #+#             */
/*   Updated: 2026/07/02 16:01:24 by anait-il         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_coder popleft(t_coder *queue, int *count)
{
    int     i;
    t_coder first;

    i = 0;
    first = queue[0];
    while (i < *count - 1)
    {
        queue[i] = queue[i+1];
        i++;
    }
    count--;
    return (first);
}

t_coder heappush(t_coder *heap, int *count)
{
    int     i;
    t_coder first;

    i = 0;
    first = heap[0];
    while (i < *count - 1)
    {

    }

}
