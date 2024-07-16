/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alouriga <alouriga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 04:35:49 by alouriga          #+#    #+#             */
/*   Updated: 2024/06/11 16:59:21 by alouriga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int check_str(char *s)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
	{
		if (!((s[i] >= '0' && s[i] <= '9')
			|| (s[i] == '+') || s[i] == '-'))
				return (-1);
		i++;
	}
	i = 0;
	while (s[i] != '\0')
	{
		if ((s[i] == '+' || s[i] == '-') && i != 0)
			return (-1);
		i++;
	}
	return (0);
}

long ft_atoi(const char *str)
{
	int					i;
	int					sign;
	long				result;
	long				result1;

	i = 0;
	sign = 1;
	result = 0;
	if (!str)
		return (0);
	if (str[i] == '+' || str[i] == '-')
		if (str[i++] == '-')
			sign *= -1;
	while (str[i] >= '0' && str[i] <= '9')
	{
		result1 = result * 10 + (str[i] - '0');
		if ((result1 > INT_MAX && sign == 1) || (result1 * sign < INT_MIN))
			return (-1);
		result = result1;
		i++;
	}
	return (result * sign);
}