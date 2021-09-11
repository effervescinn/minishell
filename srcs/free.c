#include "minishell.h"

void	free_tokens(t_info *info)
{
	int	i;

	i = 0;
	while (info->tokens[i].str)
	{
		free(info->tokens[i].str);
		i++;
	}
	free(info->tokens);
}

void	free_args(t_info *info)
{
	int	i;
	int	j;

	i = 0;
	while (info->tokens[i].str)
	{
		j = 0;
		while (info->tokens[i].args[j])
		{
			free(info->tokens[i].args[j]);
			j++;
		}
		free(info->tokens[i].args);
		i++;
	}
}

void	free_list(t_list **list)
{
	t_list	*tmp;

	while (*list)
	{
		tmp = *list;
		*list = (*list)->next;
		free(tmp->content);
		free(tmp);
	}
	*list = NULL;
}
