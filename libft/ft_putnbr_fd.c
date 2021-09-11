/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnorcros <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/11 13:48:58 by lnorcros          #+#    #+#             */
/*   Updated: 2020/11/11 13:49:00 by lnorcros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_divider(long long int c)
{
	long long int	divider;

	divider = 1;
	while (c > 9)
	{
		c /= 10;
		divider *= 10;
	}
	return (divider);
}

void	ft_putnbr_fd(int n, int fd)
{
	long long int	new_n;
	long long int	div;
	char			c;

	new_n = (long long int)n;
	if (new_n < 0)
	{
		write(fd, "-", 1);
		new_n = new_n * (-1);
	}
	div = ft_divider(new_n);
	while (div)
	{
		c = (new_n / div) + '0';
		write(fd, &c, 1);
		new_n -= (new_n / div) * div;
		div /= 10;
	}
}
