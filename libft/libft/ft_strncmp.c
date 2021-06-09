/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnorcros <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/11 13:51:35 by lnorcros          #+#    #+#             */
/*   Updated: 2020/11/11 13:51:37 by lnorcros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned const char	*new_s1;
	unsigned const char	*new_s2;

	new_s1 = (unsigned const char*)s1;
	new_s2 = (unsigned const char*)s2;
	if (n == 0)
		return (0);
	while (*new_s1 != '\0' && *new_s2 != '\0' && *new_s1 == *new_s2 && n > 1)
	{
		new_s1++;
		new_s2++;
		n--;
	}
	if (*new_s1 == *new_s2)
		return (0);
	else
		return (*new_s1 - *new_s2);
}
