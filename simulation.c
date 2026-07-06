/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anait-il <your@mail.com>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 15:58:31 by anait-il          #+#    #+#             */
/*   Updated: 2026/07/02 15:58:31 by anait-il         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
int simulation(t_coder *coder)
{
    return (0);
}

int all_thread_ready(t_coder coder)
{
    if (coder.id == coder.program.data.number_of_coders)
        return (1);
    return (0);
}
