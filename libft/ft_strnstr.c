/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnorcros <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/11 13:52:55 by lnorcros          #+#    #+#             */
/*   Updated: 2020/11/11 13:53:01 by lnorcros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	needle_len;
	size_t	len_dif;

	needle_len = ft_strlen(needle);
	if (len == 0 && needle_len == 0)
		return ((char *)haystack);
	if (len == 0)
		return (NULL);
	if (needle_len == 0)
		return ((char *)haystack);
	if (needle_len > ft_strlen(haystack))
		return (NULL);
	len_dif = len - needle_len + 1;
	while (len_dif--)
	{
		if (ft_strncmp(haystack, needle, needle_len) != 0)
			haystack++;
		else
			return ((char *)haystack);
	}
	return (NULL);
}
