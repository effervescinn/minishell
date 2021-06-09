/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnorcros <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/11 13:50:53 by lnorcros          #+#    #+#             */
/*   Updated: 2020/11/11 13:50:55 by lnorcros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char	*new_s1;
	const unsigned char	*new_s2;

	new_s1 = (const unsigned char*)s1;
	new_s2 = (const unsigned char*)s2;
	while (n--)
	{
		if (*new_s1 != *new_s2)
			return (*new_s1 - *new_s2);
		else
		{
			new_s1++;
			new_s2++;
		}
	}
	return (0);
}
