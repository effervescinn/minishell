/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnorcros <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/11 13:53:25 by lnorcros          #+#    #+#             */
/*   Updated: 2020/11/11 13:53:27 by lnorcros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char		*new_dst;
	const unsigned char	*new_src;

	new_dst = (unsigned char*)dst;
	new_src = (const unsigned char*)src;
	if (new_dst == NULL && new_src == NULL)
		return (dst);
	if (src < dst)
	{
		while (len--)
			new_dst[len] = new_src[len];
	}
	else
	{
		while (len--)
		{
			*new_dst = *new_src;
			new_dst++;
			new_src++;
		}
	}
	return (dst);
}
