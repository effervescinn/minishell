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

static int	count_words(char const *s, char c)
{
	int	w_q;

	w_q = 0;
	while (*s)
	{
		while (*s == c)
		{
			s++;
			if (*s == '\0')
				return (w_q);
		}
		while (*s != c)
		{
			s++;
			if (*s == '\0')
				return (++w_q);
		}
		w_q++;
		s++;
	}
	return (w_q);
}

static int	free_all(char **arr, int i, int w_len)
{
	if (!(arr[i] = (char *)malloc(w_len * sizeof(char) + sizeof(char))))
	{
		while (i-- >= 0)
			free(arr[i]);
		free(arr);
		return (1);
	}
	return (0);
}

static int	push_strs(const char *s, char c, char **arr, int w_q)
{
	char	*w_start;
	int		w_len;
	int		i;

	i = 0;
	while (*s && i < w_q)
	{
		w_len = 0;
		while (*s == c && *s)
			s++;
		if (*s)
			w_start = (char *)s;
		while (*s != c && *s)
		{
			w_len++;
			s++;
		}
		if ((free_all(arr, i, w_len) == 1))
			return (1);
		ft_strlcpy(arr[i], w_start, w_len + 1);
		i++;
	}
	arr[i] = NULL;
	return (0);
}

char		**ft_split(char const *s, char c)
{
	char	**arr;
	int		w_q;

	if (s == NULL)
		return (NULL);
	w_q = count_words(s, c);
	if ((arr = (char **)malloc(w_q * sizeof(char *) + sizeof(char *))))
	{
		if ((push_strs(s, c, arr, w_q) == 1))
			return (NULL);
		return (arr);
	}
	return (NULL);
}
