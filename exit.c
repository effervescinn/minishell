#include "minishell.h"

int	exit_check(t_info *info)
{
	int	ex;
	int	i;

	i = 0;
	ex = 0;
	if (info->tokens[info->i + 1].str)
	{
		while (info->tokens[info->i + 1].str[i])
		{
			if (!ft_isdigit(info->tokens[info->i + 1].str[i]))
			{
				write(2, "-dashBash: exit: ", 17);
				write(2, info->tokens[info->i + 1].str,
					ft_strlen(info->tokens[info->i + 1].str));
				write(2, ": numeric argument required\n", 28);
				break ;
			}
			i++;
		}
		if (!info->tokens[info->i + 1].str[i])
			ex = ft_atoi(info->tokens[info->i + 1].str);
	}
	return (ex);
}

void	exit_minishell(t_info *info)
{
	int	ex;

	write(1, "exit\n", 6);
	ex = exit_check(info);
	free_list(&info->extra_exp);
	free_list(&info->head);
	free_args(info);
	free_tokens(info);
	free(info->str_pwd);
	free(info->str_oldpwd);
	exit(ex);
}

void	exit_ctrl_d(t_info *info)
{
	free_list(&info->extra_exp);
	free_list(&info->head);
	free(info->str_pwd);
	free(info->str_oldpwd);
	free(g_global.prompt);
	write(1, "exit\n", 5);
	exit(0);
}
