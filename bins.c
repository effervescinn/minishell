#include "minishell.h"

void free_paths_array(t_info *info)
{
    int i;

    if (!info->pths_array)  
        return;
    i = 0;
    while (info->pths_array[i++])
        free(info->pths_array[i]);
    free(info->pths_array);
    info->pths_array = NULL;
}

void make_paths(t_info *info)
{
    if (info->pths)
        info->pths_array = ft_split(info->pths->content + 6, ':');
    else
        info->pths_array = NULL;
}

char *find_bin(t_info *info) //info->pths_array
{
	struct stat buf;
	char *ret;
    int i;
    char *tmp;

	i =-1;
    while (info->pths_array[++i])
    {
        tmp = info->pths_array[i];
        info->pths_array[i] = ft_strjoin(tmp, "/");
        free(tmp);
        tmp = info->pths_array[i];
		info->pths_array[i] = ft_strjoin(tmp, info->tokens[info->i].str);
        free(tmp);
    }
    i =-1;
	while (info->pths_array[++i])
	{
		if (stat(info->pths_array[i], &buf) == 0)
		{
			ret = ft_strdup(info->pths_array[i]);
			return (ret);
		}
	}
	return (NULL);
}
