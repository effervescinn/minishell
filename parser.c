#include "minishell.h"

void	set_args(t_info *info)
{
	int	i;

	i = 0;
	while (info->tokens[i].str)
	{
		info->tokens[i].args = (char **)malloc(sizeof(char *));
		info->tokens[i].args[0] = NULL;
		i++;
	}
}

void	set_pipes(t_info *info)
{
	int	i;
	int	j;

	i = -1;
	info->pipes_num = 0;
	while (info->tokens[++i].str)
		info->tokens[i].pipe = 0;
	i = -1;
	while (info->tokens[++i].str)
	{
		if (info->tokens[i].type == 'c')
		{
			j = i + 1;
			while (info->tokens[j].str)
			{
				if (info->tokens[j].type == 'p')
					info->tokens[i].pipe = 1;
				j++;
			}
		}
	}
	i = -1;
	while (info->tokens[++i].str)
		if (info->tokens[i].type == 'p')
			info->pipes_num++;
}

void	command_types(t_info *info)
{
	int	i;

	i = 0;
	while (info->tokens[i].str)
	{
		if ((!ft_strncmp(info->tokens[i].str, "cd", 2)
				&& ft_strlen(info->tokens[i].str) == 2
				&& info->tokens[i].type == 'c')
			|| (ft_strlen(info->tokens[i].str) == 6
				&& !ft_strncmp(info->tokens[i].str, "export", 6)
				&& info->tokens[i].type == 'c' && info->tokens[i].args[1])
			|| (ft_strlen(info->tokens[i].str) == 5
				&& !ft_strncmp(info->tokens[i].str, "unset", 5)
				&& info->tokens[i].type == 'c')
			|| (ft_strlen(info->tokens[i].str) == 4
				&& !ft_strncmp(info->tokens[i].str, "exit", 4)
				&& info->tokens[i].type == 'c'))
			info->tokens[i].print = 0;
		i++;
	}
}
