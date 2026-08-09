/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_functions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anait-il <anait-il@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 14:36:12 by anait-il          #+#    #+#             */
/*   Updated: 2026/08/06 16:00:59 by anait-il         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long long	ft_atoi(char *str)
{
	int			i;
	long long	res;

	i = 0;
	res = 0;
	if (!str)
		return (-1);
	if (str[i] == '-')
	{
		fprintf(stderr, "Error: negative numbers not allowed '%s'\n", str);
		return (-1);
	}
	if (str[i] == '+')
		i++;
	while (str[i] != '\0')
	{
		res = res * 10 + (str[i] - '0');
		if (res > 2147483647)
		{
			fprintf(stderr, "Error: argument exceeds INT_MAX '%s'\n", str);
			return (-1);
		}
		i++;
	}
	return (res);
}

int	ft_isdigit(int i)
{
	if (i >= '0' && i <= '9')
		return (1);
	return (0);
}

int	is_space(int i)
{
	if (i == ' ' || (i <= 13 && i >= 9))
		return (1);
	return (0);
}
