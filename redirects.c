#include "minishell.h"

void	found_redir_buildin(t_info *info, int i, int *fd)
{
	int	a;

	if (info->tokens[i].type == 'g')
	{
		a = write(*fd, NULL, 0);
		if (*fd)
			close(*fd);
		*fd = open(info->tokens[i].args[0], O_CREAT | O_WRONLY | O_TRUNC, 0777);
		a = write(*fd, NULL, 0);
		if (a == -1)
			opening_error(info->tokens[i].args[0]);
	}
	if (info->tokens[i].type == 'G')
	{
		a = write(*fd, NULL, 0);
		if (*fd)
			close(*fd);
		*fd = open(info->tokens[i].args[0], O_CREAT | O_WRONLY | O_APPEND,
				0777);
		a = write(*fd, NULL, 0);
		if (a == -1)
			opening_error(info->tokens[i].args[0]);
	}
}

int	define_fd_built_in(t_info *info)
{
	int	i;
	int	fd;

	i = info->i2;
	fd = -1;
	while (info->tokens[i].str && info->tokens[i].type != 'p')
	{
		found_redir_buildin(info, i, &fd);
		i++;
	}
	if (fd == -1)
		return (1);
	return (fd);
}

int	count_files(t_info *info, int q)
{
	int	files;
	int	q2;
	int	i;

	files = 0;
	q2 = 0;
	i = info->i2;
	while (info->tokens[i].str && info->tokens[i].type != 'p')
	{
		if (info->tokens[i].type == 'l')
		{
			if (q == q2)
			{
				while (info->tokens[i].args[files])
					files++;
				return (files);
			}
			q2++;
		}
		i++;
	}
	return (files);
}

int	count_redir(t_info *info)
{
	int	i;
	int	smb;

	i = info->i2;
	smb = 0;
	while (info->tokens[i].str && info->tokens[i].type != 'p')
	{
		if (info->tokens[i].type == 'l')
			smb++;
		i++;
	}
	return (smb);
}
