/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anait-il <anait-il@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/31 10:59:53 by anait-il          #+#    #+#             */
/*   Updated: 2026/08/03 17:07:54 by anait-il         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_time_ms(void)
{
	struct timeval	tv;
	long			time;

	gettimeofday(&tv, NULL);
	time = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (time);
}

long	get_elapsed_ms(long start)
{
	return (get_time_ms() - start);
}

void	my_sleep(long time, t_program *program)
{
	long	start;

	start = get_time_ms();
	while (get_time_ms() - start < time)
	{
		usleep(500);
		if (!is_running(program))
			return ;
	}
}
