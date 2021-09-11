/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnorcros <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/11 13:49:42 by lnorcros          #+#    #+#             */
/*   Updated: 2020/11/11 13:49:44 by lnorcros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*new_s;
	unsigned char	new_c;

	new_s = (unsigned char *)s;
	new_c = (unsigned char)c;
	while (n)
	{
		if (*new_s == new_c)
			return ((void *)new_s);
		new_s++;
		n--;
	}
	return (NULL);
}
