#include "minishell.h"

void	add_from_the_first(t_info *info, int q, char **array, int *k)
{
	int	q2;
	int	files;
	int	i;

	i = info->i2;
	files = 1;
	q2 = 0;
	while (info->tokens[i].str && info->tokens[i].type != 'p')
	{
		if (info->tokens[i].type == 'l')
		{
			if (q == q2)
			{
				while (info->tokens[i].args[files] && info->tokens[i].args[0])
				{
					array[*k] = ft_strdup(info->tokens[i].args[files]);
					(*k)++;
					files++;
				}
				return ;
			}
			q2++;
		}
		i++;
	}
}

void	add_redirect_args(char **array, int *k, t_info *info, int smb)
{
	int	q;

	q = 0;
	while (q < smb)
	{
		add_from_the_first(info, q, array, k);
		q++;
	}
}

char	**create_new_exec_array(t_info *info, int files, int smb)
{
	char	**array;
	int		k;

	k = 0;
	while (info->tokens[info->i].args[k])
		k++;
	array = (char **)malloc(sizeof(char *) * (k + files + 1));
	k = 0;
	while (info->tokens[info->i].args[k])
	{
		array[k] = ft_strdup(info->tokens[info->i].args[k]);
		k++;
	}
	add_redirect_args(array, &k, info, smb);
	array[k] = NULL;
	k = 0;
	while (info->tokens[info->i].args[k])
	{
		free(info->tokens[info->i].args[k]);
		k++;
	}
	free(info->tokens[info->i].args);
	return (array);
}
