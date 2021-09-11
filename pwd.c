#include "minishell.h"

void	pwd(t_info *info)
{
	char	*buf;

	buf = NULL;
	buf = getcwd(NULL, 100);
	if (buf)
	{
		info->result = no_leaks_join(info->result, buf);
		info->result = no_leaks_join(info->result, "\n");
	}
	while (!buf)
	{
		chdir("..");
		buf = getcwd(NULL, 100);
		info->result = no_leaks_join(info->result, buf);
		info->result = no_leaks_join(info->result, "\n");
	}
	free(buf);
}
