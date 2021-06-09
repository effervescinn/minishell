/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnorcros <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/11 13:53:37 by lnorcros          #+#    #+#             */
/*   Updated: 2020/11/11 13:53:39 by lnorcros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*ptr;

	if ((ptr = (char *)malloc(ft_strlen(s1) * sizeof(*s1) + sizeof(*s1))))
	{
		ft_memcpy(ptr, s1, ft_strlen(s1));
		*(ptr + ft_strlen(s1)) = '\0';
		return (ptr);
	}
	return (NULL);
}
