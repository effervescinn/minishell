/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnorcros <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/11 13:45:44 by lnorcros          #+#    #+#             */
/*   Updated: 2020/11/11 13:45:46 by lnorcros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*res_str;
	size_t	s1_len;
	size_t	s2_len;

	if (s1 == NULL)
		return (NULL);
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	res_str = (char *)malloc((s1_len + s2_len) * sizeof(*s1) + 1);
	if (res_str)
	{
		ft_bzero(res_str, ft_strlen(s1) + ft_strlen(s2) + 1);
		ft_memmove(res_str, s1, ft_strlen(s1));
		res_str = res_str + ft_strlen(s1);
		ft_memmove(res_str, s2, ft_strlen(s2));
		return (res_str - ft_strlen(s1));
	}
	return (NULL);
}
