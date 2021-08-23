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

char *find_bin(t_info *info)
{
	struct stat buf;
	char *ret;
    int i;
    char *tmp;
    char **tmp_arr;
    int j;

	i = 0;
    while (info->pths_array[i])
        i++;
    tmp_arr = (char**)malloc(sizeof(char*) * (i + 1));
    tmp_arr[i] = NULL;
    i =-1;
    while (info->pths_array[++i])
    {
        tmp = ft_strjoin(info->pths_array[i], "/");
        tmp_arr[i] = ft_strjoin(tmp, info->tokens[info->i].str);
        free(tmp);
    }
    i =-1;
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
	return (NULL);
}

void define_fd_out(t_info *info)
{
    int i;
    int fd;
    int flag;

    flag = 0;
    i = 0;
    fd = -5;
    while (info->tokens[i].str && info->tokens[i].type[0] != 'p')
    {
        if (info->tokens[i].type[0] == 'g')
        {
            write(fd, "\0", 1);
            close(fd);
            fd = open(info->tokens[i].args[0], O_CREAT | O_WRONLY | O_TRUNC, 0777);
            flag = 1;
        }
        if (info->tokens[i].type[0] == 'G')
        {
            write(fd, "\0", 1);
            close(fd);
            fd = open(info->tokens[i].args[0], O_CREAT | O_WRONLY | O_APPEND, 0777);
            flag = 1;
        }
        i++;
    }
    if (!flag)
        return;
    dup2(fd, 1);
    close(fd);
}


void define_fd_in(t_info *info)
{
    int i;
    int fd;
    int flag;

    flag = 0;
    i = 0;
    fd = -5;
    while (info->tokens[i].str && info->tokens[i].type[0] != 'p')
    {
        if (info->tokens[i].type[0] == 'l')
        {
            close(fd);
            fd = open(info->tokens[i].args[0], O_RDONLY);
            flag = 1;
        }
        i++;
    }
    if (!flag)
        return;
    dup2(fd, 0);
}

int define_fd_built_in(t_info *info)
{
    int i;
    int fd;

    i = 0;
    fd = -5;
    while (info->tokens[i].str && info->tokens[i].type[0] != 'p')
    {
        if (info->tokens[i].type[0] == 'g')
        {
            write (fd, "\0", 1);
            close(fd);
            fd = open(info->tokens[i].args[0], O_CREAT | O_WRONLY | O_TRUNC, 0777);
        }
        if (info->tokens[i].type[0] == 'G')
        {
            write(fd, "\0", 1);
            close(fd);
            fd = open(info->tokens[i].args[0], O_CREAT | O_WRONLY | O_APPEND, 0777);
        }
        i++;
    }
    if (fd == -5)
        return (1);
    return (fd);
}
