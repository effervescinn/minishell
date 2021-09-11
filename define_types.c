#include "minishell.h"

void	define_command(t_info *info, int *i)
{
	info->tokens[*i].type = 'c';
	free(info->tokens[*i].args[0]);
	free(info->tokens[*i].args);
	command_args(info->tokens, *i);
}

void	define_pipe(t_info *info, int *i)
{
	if (info->tokens[*i + 1].str)
		if (info->tokens[*i + 1].type == 'w')
			info->tokens[*i + 1].type = 'c';
}

void	define_great(t_info *info, int *i)
{
	free(info->tokens[*i].args[0]);
	free(info->tokens[*i].args);
	info->tokens[*i].args = (char **)malloc(sizeof(char *) * 2);
	info->tokens[*i].args[0] = ft_strdup(info->tokens[*i + 1].str);
	info->tokens[*i].args[1] = NULL;
	if (!((i != 0) && (info->tokens[*i - 1].type != 'p')))
		if (info->tokens[*i + 2].str)
			if (info->tokens[*i + 2].type == 'w')
				info->tokens[*i + 2].type = 'c';
}

void	define_types(t_info *info)
{
	int	i;

	i = 0;
	while (info->tokens[i].str)
	{
		info->tokens[i].print = 1;
		if ((i == 0 && info->tokens[i].type == 'w')
			|| info->tokens[i].type == 'c')
			define_command(info, &i);
		else if (info->tokens[i].type == 'p')
			define_pipe(info, &i);
		else if (info->tokens[i].type == 'g' || info->tokens[i].type == 'G')
			define_great(info, &i);
		else if (info->tokens[i].type == 'l')
		{
			free(info->tokens[i].args[0]);
			free(info->tokens[i].args);
			less_args(info->tokens, i);
		}
		else if (info->tokens[i].type == 'L')
			define_greatless(info, &i);
		i++;
	}
}
