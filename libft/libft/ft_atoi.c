/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnorcros <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/11 13:47:02 by lnorcros          #+#    #+#             */
/*   Updated: 2020/11/11 13:47:04 by lnorcros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_space(const char s)
{
	if (s == ' ' || s == '\n' || s == '\t'
	|| s == '\v' || s == '\f' || s == '\r')
		return (1);
	return (0);
}

static int	is_num(const char s)
{
	if (s >= '0' && s <= '9')
		return (1);
	return (0);
}

int			ft_atoi(const char *str)
{
	unsigned long long int	res;
	int						sign;

	res = 0;
	sign = 1;
	while (is_space(*str))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (is_num(*str))
	{
		res = res * 10 + *str - '0';
		str++;
	}
	if (res > 999999999999999999 && sign == 1)
		return (-1);
	if (res > 999999999999999999 && sign == -1)
		return (0);
	return (res * sign);
}
