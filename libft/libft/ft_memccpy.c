/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnorcros <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/11 13:48:40 by lnorcros          #+#    #+#             */
/*   Updated: 2020/11/11 13:48:42 by lnorcros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memccpy(void *dst, const void *src, int c, size_t n)
{
	unsigned char			*new_dst;
	const unsigned char		*new_src;
	unsigned char			s;

	new_dst = (unsigned char*)dst;
	new_src = (const unsigned char*)src;
	s = (unsigned char)c;
	if (new_dst == NULL && new_src == NULL)
		return (dst);
	while (n)
	{
		*new_dst = *new_src;
		new_dst++;
		if (*new_src == s)
			return ((void*)new_dst);
		new_src++;
		n--;
	}
	return (NULL);
}
