/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnorcros <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/11 13:52:22 by lnorcros          #+#    #+#             */
/*   Updated: 2020/11/11 13:52:28 by lnorcros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	char			*new_s;
	unsigned char	new_c;

	new_s = (char *)s;
	new_c = (unsigned char)c;
	while (*new_s)
	{
		if (*new_s == new_c)
			return (new_s);
		new_s++;
	}
	if (*new_s == '\0' && new_c == '\0')
		return (new_s);
	return (NULL);
}
