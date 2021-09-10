#include "minishell.h"

void free_paths_array(t_info *info)
{
    int i;

    if (!info->pths_array)  
        return;
    i = -1;
    while (info->pths_array[++i])
        free(info->pths_array[i]);
    free(info->pths_array);
    info->pths_array = NULL;
}

void make_paths(t_info *info)
{
    if (info->pths){
		free_paths_array(info);
        info->pths_array = ft_split(info->pths->content + 5, ':');}
    else
		free_paths_array(info);
}

char *find_bin_return(char **tmp_arr)
{
    int i;
    char *ret;
    int j;
    struct stat buf;

    i = -1;
    while (tmp_arr[++i])
    {
        if (stat(tmp_arr[i], &buf) == 0)
        {
            ret = ft_strdup(tmp_arr[i]);
            j = -1;
            while (tmp_arr[++j])
                free(tmp_arr[j]);
            free(tmp_arr);
            return (ret);
        }
    }
    j = -1;
    while (tmp_arr[++j])
        free(tmp_arr[j]);
    free(tmp_arr);
	return (NULL);
}

char *find_bin(t_info *info)
{
	char *ret;
    int i;
    char *tmp;
    char **tmp_arr;

    if (!ft_strncmp("./", info->tokens[info->i].str, 2))
    {
        ret = ft_strdup(info->tokens[info->i].str + 2);
        return (ret);
    }
	if (!info->pths_array)
        return (NULL);
    i = 0;
    while (info->pths_array[i])
        i++;
    tmp_arr = (char**)malloc(sizeof(char*) * (i + 1));
    tmp_arr[i] = NULL;
    i = -1;
    while (info->pths_array[++i])
    {
        tmp = ft_strjoin(info->pths_array[i], "/");
        tmp_arr[i] = ft_strjoin(tmp, info->tokens[info->i].str);
        free(tmp);
    }
    return (find_bin_return(tmp_arr));
}
