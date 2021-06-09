/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnorcros <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/11 13:53:52 by lnorcros          #+#    #+#             */
/*   Updated: 2020/11/11 13:53:55 by lnorcros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	unsigned char	*new_s;
	unsigned char	new_c;
	size_t			i;

	new_s = (unsigned char *)s;
	new_c = (unsigned char)c;
	i = ft_strlen((const char *)new_s);
	new_s = new_s + i;
	while (new_s >= (unsigned char *)s)
	{
		if (*new_s == new_c)
			return ((char *)new_s);
		new_s--;
	}
	return (NULL);
}
