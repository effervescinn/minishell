/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnorcros <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/11 13:46:35 by lnorcros          #+#    #+#             */
/*   Updated: 2020/11/11 13:46:38 by lnorcros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	char		*new_dst;
	const char	*new_src;
	size_t		dst_len;
	size_t		src_len;

	new_dst = dst;
	new_src = src;
	dst_len = ft_strlen(dst);
	src_len = ft_strlen(src);
	new_dst = new_dst + dst_len;
	if (dstsize < dst_len)
		return (src_len + dstsize);
	if (dstsize == dst_len)
		return (dst_len + src_len);
	dstsize = dstsize - dst_len - 1;
	while (dstsize-- && src_len--)
	{
		*new_dst = *new_src;
		new_dst++;
		new_src++;
	}
	*new_dst = '\0';
	return (dst_len + ft_strlen(src));
}
