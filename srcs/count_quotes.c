#include "../minishell.h"

int find_double(char **str, int *quotes)
{
	(*str)++;
	(*quotes)++;
	*str = ft_strchr(*str, '\"');
	if (!*str)
		return (*quotes);
	(*quotes)++;
	(*str)++;
	return (0);
}

int find_single(char **str, int *quotes)
{
	(*str)++;
	(*quotes)++;
	*str = ft_strchr(*str, '\'');
	if (!*str)
		return (*quotes);
	(*quotes)++;
	(*str)++;
	return (0);
}

int count_quotes(char *str)
{
	int quotes;
	int ret;

	quotes = 0;
	while (*str)
	{
		while (*str && *str != '\'' && *str != '\"')
			str++;
		if (*str == '\"')
		{
			ret = find_double(&str, &quotes);
			if (ret)
				return (ret);
		}
		else if (*str == '\'')
		{
			ret = find_single(&str, &quotes);
			if (ret)
				return (ret);
		}
		if (*str == '\0')
			continue;
	}
	return (quotes);
}
