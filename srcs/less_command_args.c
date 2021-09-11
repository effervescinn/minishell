#include "minishell.h"

void	less_command(t_token **tokens, int *i)
{
	(*tokens)[*i].args = (char **)malloc(sizeof(char *) * 2);
	(*tokens)[*i].args[1] = NULL;
	(*tokens)[*i].args[0] = ft_strdup((*tokens)[*i + 1].str);
	if ((*tokens)[*i + 2].str)
		if ((*tokens)[*i + 2].type == 'w')
			(*tokens)[*i + 2].type = 'c';
}

void	less_args(t_token *tokens, int i)
{
	int	q;
	int	j;

	if ((i != 0) && (tokens[i - 1].type != 'p'))
	{
		q = 0;
		i++;
		while (tokens[i].str && tokens[i].type == 'w')
		{
			i++;
			q++;
		}
		i -= q + 1;
		tokens[i].args = (char **)malloc(sizeof(char *) * (q + 1));
		tokens[i].args[q] = NULL;
		j = 0;
		while (q)
		{
			tokens[i].args[j] = ft_strdup(tokens[i + j + 1].str);
			j++;
			q--;
		}
	}
	else
		less_command(&tokens, &i);
}

void	define_greatless(t_info *info, int *i)
{
	free(info->tokens[*i].args[0]);
	free(info->tokens[*i].args);
	info->tokens[*i].args = (char **)malloc(sizeof(char *) * 2);
	info->tokens[*i].args[0] = ft_strdup(info->tokens[*i + 1].str);
	info->tokens[*i].args[1] = NULL;
}

void	set_command_args(t_token **tokens, int *i, int *j, int *q)
{
	int	k;

	(*tokens)[*j].args = (char **)malloc(sizeof(char *) * (*q + 2));
	(*tokens)[*j].args[*q + 1] = NULL;
	(*tokens)[*j].args[0] = ft_strdup((*tokens)[*j].str);
	k = 0;
	while ((*tokens)[*j + k + 1].str && (*tokens)[*j + k + 1].type == 'w')
	{
		(*tokens)[*j].args[k + 1] = ft_strdup((*tokens)[*j + k + 1].str);
		k++;
	}
	*i = *j + k + 1;
	if ((*tokens)[*i].str)
	{
		if ((*tokens)[*i].type == 'g' || (*tokens)[*i].type == 'G')
		{
			*i += 2;
			while ((*tokens)[*i].str && (*tokens)[*i].type == 'w')
			{
				(*tokens)[*j].args[k + 1] = ft_strdup((*tokens)[*i].str);
				k++;
				(*i)++;
			}
		}
	}
}

void	command_args(t_token *tokens, int i)
{
	int	q;
	int	j;

	q = 0;
	j = i;
	i++;
	while (tokens[i].str && tokens[i].type == 'w')
	{
		i++;
		q++;
	}
	if (tokens[i].str)
	{
		if (tokens[i].type == 'g' || tokens[i].type == 'G')
		{
			i += 2;
			while (tokens[i].str && tokens[i].type == 'w')
			{
				i++;
				q++;
			}
		}
	}
	set_command_args(&tokens, &i, &j, &q);
}
