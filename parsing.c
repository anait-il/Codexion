/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anait-il <your@mail.com>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 14:37:07 by anait-il          #+#    #+#             */
/*   Updated: 2026/07/11 14:37:14 by anait-il         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_valid(char *av[])
{
	int	i;
	int	y;

	i = 1;
	while (av[i])
	{
		y = 0;
		if (i < 8)
		{
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
		}
		else
		{
			while (av[i][y] == ' ' || (av[i][y] <= 13))
				y++;
			if (!strcmp(av[i], "fifo"))
				return (0);
			else if (!strcmp(av[i], "edf"))
				return (0);
			else
				return (i);
		}
		i++;
	}
	return (0);
}

int	parsing(int ac, char *av[], t_program *program)
{
	int	parsing_stat;

	if (ac != 9)
	{
		fprintf(stderr, "Error missing arguments: number of arguments is not correct ");
		return (1);
	}
	parsing_stat = is_valid(av);
	if (parsing_stat)
	{
		fprintf(stderr, "Error: invalid argument '%s'", av[parsing_stat]);
		return (1);
	}
    printf("%s number of coders", av[1]);
	program->data.number_of_coders = ft_atoi(av[1]);
	program->data.time_to_burnout = ft_atoi(av[2]);
	program->data.time_to_compile = ft_atoi(av[3]);
	program->data.time_to_debug = ft_atoi(av[4]);
	program->data.time_to_refactor = ft_atoi(av[5]);
	program->data.number_of_compiles_required = ft_atoi(av[6]);
	program->data.dongle_cooldown = ft_atoi(av[7]);
	program->data.scheduler = av[8];
	return (0);
}
