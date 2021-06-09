/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnorcros <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/11 13:47:13 by lnorcros          #+#    #+#             */
/*   Updated: 2020/11/11 13:47:15 by lnorcros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_len(long long int n)
{
	int	n_len;

	n_len = 0;
	while (n)
	{
		n_len++;
		n /= 10;
	}
	return (n_len);
}

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

static char	*itos(long long int new_n, char *res)
{
	int	div;
	int	i;

	i = -1;
	if (new_n < 0)
	{
		res[++i] = '-';
		new_n = new_n * (-1);
	}
	div = ft_divider(new_n);
	while (div)
	{
		res[++i] = (new_n / div) + '0';
		new_n -= (new_n / div) * div;
		div /= 10;
	}
	res[++i] = '\0';
	return (res);
}

char		*ft_itoa(int n)
{
	char			*res;
	long long int	n_len;
	long long int	new_n;

	new_n = n;
	n_len = count_len(new_n);
	if ((res = (char *)malloc(n_len * sizeof(char) + 2 * sizeof(char))))
	{
		return (itos(new_n, res));
	}
	return (NULL);
}
