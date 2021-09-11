/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnorcros <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/11 13:50:05 by lnorcros          #+#    #+#             */
/*   Updated: 2020/11/11 13:50:06 by lnorcros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*new_s;
	size_t			s_len;
	unsigned int	i;

	if (s == NULL)
		return (NULL);
	s_len = ft_strlen(s);
	i = 0;
	new_s = (char *)malloc(s_len * sizeof(*s) + 1);
	if (new_s)
	{
		while (i < s_len)
		{
			new_s[i] = f(i, s[i]);
			i++;
		}
		new_s[i] = '\0';
		return (new_s);
	}
	return (NULL);
}
