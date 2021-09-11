#include "minishell.h"

int	check_flags(char **tmp_token, int *i, int *flag)
{
	if ((*tmp_token)[*i] == '\'' && *flag == 0)
	{
		*flag = 1;
		(*i)++;
		return (1);
	}
	else if ((*tmp_token)[*i] == '\'' && *flag == 1)
	{
		*flag = 0;
		(*i)++;
		return (1);
	}
	else if ((*tmp_token)[*i] == '\"' && *flag == 0)
	{
		*flag = 2;
		(*i)++;
		return (1);
	}
	else if ((*tmp_token)[*i] == '\"' && *flag == 2)
	{
		*flag = 0;
		(*i)++;
		return (1);
	}
	return (0);
}

void	s_flag(char **tmp_token, char **str, int *i, int *j)
{
	while ((*tmp_token)[*i] && (*tmp_token)[*i] != '\'')
	{
		(*str)[*j] = (*tmp_token)[*i];
		(*i)++;
		(*j)++;
	}
}

void	d_flag(char **tmp_token, char **str, int *i, int *j)
{
	while ((*tmp_token)[*i] && (*tmp_token)[*i] != '\"')
	{
		(*str)[*j] = (*tmp_token)[*i];
		(*i)++;
		(*j)++;
	}
}

void	no_flags(char **tmp_token, char **str, int *i, int *j)
{
	while ((*tmp_token)[*i] && (*tmp_token)[*i]
	!= '\"' && (*tmp_token)[*i] != '\'')
	{
		(*str)[*j] = (*tmp_token)[*i];
		(*i)++;
		(*j)++;
	}
}
