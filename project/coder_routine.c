/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anait-il <anait-il@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 18:35:36 by anait-il          #+#    #+#             */
/*   Updated: 2026/08/04 15:53:13 by anait-il         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	compile(t_coder *coder)
{
	pthread_mutex_lock(&coder->program->monitor_lock);
	coder->compile_counter++;
	coder->last_compile_time = get_elapsed_ms(coder->program->start_time);
	pthread_mutex_unlock(&coder->program->monitor_lock);
	log_state(coder, "is compiling\n");
	my_sleep(coder->program->data.time_to_compile, coder->program);
}

static void	debug(t_coder *coder)
{
	log_state(coder, "is debugging\n");
	my_sleep(coder->program->data.time_to_debug, coder->program);
}

static void	refactore(t_coder *coder)
{
	log_state(coder, "is refactoring\n");
	my_sleep(coder->program->data.time_to_refactor, coder->program);
}

static void	coder_cycle(t_coder *coder)
{
	if (is_running(coder->program))
	{
		compile(coder);
		release_dongles(coder);
	}
	if (is_running(coder->program))
		debug(coder);
	if (is_running(coder->program))
		refactore(coder);
}

void	*coder_routine(void *arg)
{
	t_coder		*coder;
	int			state;

	coder = (t_coder *)arg;
	wait_for_start(coder);
	if (!is_running(coder->program))
		return (NULL);
	run_even_only(coder);
	while (is_running(coder->program))
	{
		if (coder->program->data.number_of_compiles_required \
			== coder->compile_counter)
			break ;
		state = acquire_dongles(coder);
		if (state)
			return (NULL);
		if (!is_running(coder->program))
			return (release_dongles(coder), NULL);
		coder_cycle(coder);
	}
	return (NULL);
}
