/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdelkabir <abdelkabir@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 14:37:07 by anait-il          #+#    #+#             */
/*   Updated: 2026/07/30 05:54:46 by abdelkabir       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	schedular_check(char *schedular)
{
	if (!strcmp(schedular, "fifo"))
		return (0);
	else if (!strcmp(schedular, "edf"))
		return (0);
	return (1);
}

static int	is_valid(char *av[])
{
	int	i;
	int	y;

	i = 1;
	while (i < 8)
	{
		y = 0;
		while (av[i][y] != '\0')
		{
			if ((av[i][y] == '+' || av[i][y] == '-') && y == 0)
				y++;
			if (av[i][y] == '\0')
				return (i);
			if (!ft_isdigit(av[i][y]))
				return (i);
			y++;
		}
		if (strlen(av[i]) == 0)
			return (i);
		i++;
	}
	return (0);
}

static int	parse_arg(char *arg, void *dst, char *type)
{
	long long	value;

	value = ft_atoi(arg);
	if (value == -1)
		return (1);
	if (!strcmp(type, "int"))
		*(int *)dst = value;
	else if (!strcmp(type, "long"))
		*(long long *)dst = value;
	return (0);
}

static int	init_args(char *av[], t_program *program)
{
	if (parse_arg(av[1], &program->data.number_of_coders, "int"))
		return (1);
	if (parse_arg(av[2], &program->data.time_to_burnout, "long"))
		return (1);
	if (parse_arg(av[3], &program->data.time_to_compile, "long"))
		return (1);
	if (parse_arg(av[4], &program->data.time_to_debug, "long"))
		return (1);
	if (parse_arg(av[5], &program->data.time_to_refactor, "long"))
		return (1);
	if (parse_arg(av[6], &program->data.number_of_compiles_required, "int"))
		return (1);
	if (parse_arg(av[7], &program->data.dongle_cooldown, "long"))
		return (1);
	program->data.scheduler = av[8];
	return (0);
}

int	parsing(int ac, char *av[], t_program *program)
{
	int	parsing_stat;

	if (ac != 9)
	{
		fprintf(stderr, "Error: number of arguments is not correct ");
		return (1);
	}
	parsing_stat = is_valid(av);
	if (parsing_stat)
	{
		fprintf(stderr, "Error: invalid argument '%s'\n", av[parsing_stat]);
		return (1);
	}
	if (schedular_check(av[8]))
	{
		fprintf(stderr,
			"Error: Schedular must be exactly either 'edf' or 'fifo'\n");
		return (1);
	}
	if (init_args(av, program))
		return (1);
	return (0);
}
