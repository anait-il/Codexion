/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anait-il <your@mail.com>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 14:36:12 by anait-il          #+#    #+#             */
/*   Updated: 2026/07/11 14:38:14 by anait-il         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "codexion.h"

long long	ft_atoi(char *str)
{
	int			i;
	int			sign;
	long long	res;

	i = 0;
	res = 0;
	sign = 1;
	if (!str)
		return (-1);
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			return (-1);
		i++;
	}
	while (str[i] != '\0')
	{
		if (res < 0)
			return (-1);
		res = res * 10 + (str[i] - '0');
		i++;
	}
	res = res * sign;
	if (res < 0)
		fprintf(stderr, "Error: negative number not allowed '%lld'", res);
	    return (-1);
	return (res * sign);
}

int	ft_isdigit(int i)
{
	if (i >= '0' && i <= '9')
		return (1);
	return (0);
}
