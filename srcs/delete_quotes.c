#include "minishell.h"

void	handle_quotes(char **tmp_token, char **str, int *i, int *j)
{
	int	flag;

	flag = 0;
	while ((*tmp_token)[*i])
	{
		if (check_flags(tmp_token, i, &flag))
			continue ;
		if (flag == 1)
		{
			s_flag(tmp_token, str, i, j);
			continue ;
		}
		else if (flag == 2)
		{
			d_flag(tmp_token, str, i, j);
			continue ;
		}
		else
		{
			no_flags(tmp_token, str, i, j);
			continue ;
		}
		if ((*tmp_token)[*i])
			(*i)++;
	}
}

void	handle_token(char *tmp_token, char **str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	handle_quotes(&tmp_token, str, &i, &j);
	(*str)[j] = '\0';
}

void	set_types(char ***tmp_arr, t_token **tokens_arr, int *i)
{
	if ((*tmp_arr)[*i][0] == '|' && (*tmp_arr)[*i][1] == '\0')
		(*tokens_arr)[*i].type = 'p';
	else if ((*tmp_arr)[*i][0] == '>' && (*tmp_arr)[*i][1] == '\0')
		(*tokens_arr)[*i].type = 'g';
	else if ((*tmp_arr)[*i][0] == '<' && (*tmp_arr)[*i][1] == '\0')
		(*tokens_arr)[*i].type = 'l';
	else if ((*tmp_arr)[*i][0] == '>' && (*tmp_arr)[*i][1]
	== '>' && (*tmp_arr)[*i][2] == '\0')
		(*tokens_arr)[*i].type = 'G';
	else if ((*tmp_arr)[*i][0] == '<' && (*tmp_arr)[*i][1]
	== '<' && (*tmp_arr)[*i][2] == '\0')
		(*tokens_arr)[*i].type = 'L';
	else
		(*tokens_arr)[*i].type = 'w';
}

t_token	*delete_quotes(char **tmp_arr)
{
	int		i;
	t_token	*tokens_arr;

	i = 0;
	while (tmp_arr[i])
		i++;
	tokens_arr = (t_token *)malloc(sizeof(t_token) * (i + 1));
	tokens_arr[i].str = NULL;
	i = -1;
	while (tmp_arr[++i])
	{
		tokens_arr[i].str = (char *)malloc(ft_strlen(tmp_arr[i]) + 1);
		handle_token(tmp_arr[i], &tokens_arr[i].str);
		set_types(&tmp_arr, &tokens_arr, &i);
	}
	return (tokens_arr);
}
