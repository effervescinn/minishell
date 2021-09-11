/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnorcros <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/11 13:44:58 by lnorcros          #+#    #+#             */
/*   Updated: 2020/11/11 13:45:03 by lnorcros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	char			*new_str;
	unsigned char	s;
	int				i;

	new_str = (char *)b;
	s = (unsigned char)c;
	i = 0;
	while (len)
	{
		new_str[i] = s;
		i++;
		len--;
	}
	return (b);
}
