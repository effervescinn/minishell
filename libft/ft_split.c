/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnorcros <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/11 19:38:03 by lnorcros          #+#    #+#             */
/*   Updated: 2020/11/11 19:38:05 by lnorcros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	int	 quantity(const char *str, char c)
{
	int	q;

	q = 0;
 	while (*str != '\0')
	{
		while (*str == c)
		{
			str++;
			if (*str == '\0')
			return(q);
		}
		if (*str != c)
		q++;
 		while (*str != c)
		{
			str++;
			if (*str == '\0')
			return (q);
		}
	}
	return (q);
}

static	int	ft_strlensep (const char *s, char c)
{
	int	len;

	len = 0;
	while (*s != c && *s != '\0')
	{
		s++;
		len++;
	}
	return (len);
}

static	void	freee(char **arr, int k)
{
	while (k--)
		free(arr[k]);
	free(arr);
}

char		**ft_split(char const *s, char c)
{
	char	**arr;
	size_t	j;
	int	k;
	int	i;

	i = 0;
	if (!s || !((arr = (char **)malloc((quantity(s, c) + 1) * sizeof(char *)))))
		return (NULL);
	k = 0;
	while (k != quantity(s, c))
	{
		while (s[i] == c && s[i] != '\0')
			i++;
		if (!(arr[k] = (char*)malloc(sizeof(char) * (ft_strlensep(&s[i], c) + 1))))
		{
			freee(arr, k);
			return (NULL);
		}
		arr[k][ft_strlensep(&s[i], c)] = '\0';
		j = 0;
		while (s[i] != c && s[i] != '\0')
			arr[k][j++] = s[i++];
		k++;
	}
	arr[k] = NULL;
	return (arr);
}