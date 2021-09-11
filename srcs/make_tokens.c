#include "../minishell.h"

void str_to_tokens(char **str, char ***arr, char **start, int *i)
{
	while (**str)
	{
		while (**str && **str == ' ')
			(*str)++;
		if (**str == '\0')
			continue;
		*start = *str;
		if (**str != '\"' && **str != '\'')
		{
			if (**str == '|' || **str == '<' || **str == '>')
			{
				pipe_redir(str, arr, start, i);
				continue;
			}
			quotes_after(str);
		}
		else if (**str == '\"' || **str == '\'')
		{
			int i = 0;

			if (*str[0] == '\'')
			{
				(*str)++;
				*str = ft_strchr(*str, '\'');
				(*str)++;
				while ((*str)[i] && (*str)[i] != ' ' && (*str)[i] != '|' && (*str)[i] != '<' && (*str)[i] != '>')
					i++;
			}
			else if (**str == '\"')
			{
				(*str)++;
				*str = ft_strchr(*str, '\"');
				(*str)++;
				while ((*str)[i] && (*str)[i] != ' ' && (*str)[i] != '|' && (*str)[i] != '<' && (*str)[i] != '>')
					i++;
			}
		}
		// meet_quotes(str);
		(*i)++;
		finish_tokens(str, arr, start, i);
	}
}

char **make_tokens(char *str)
{
	char **arr;
	char *start;
	int i;

	start = str;
	i = 0;
	arr = (char **)malloc(sizeof(char *));
	arr[0] = NULL;
	str_to_tokens(&str, &arr, &start, &i);
	return (arr);
}
