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
    tmp_arr = (char**)malloc(sizeof(char*) * (i + 1));////течет
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
    i = info->i2;
    fd = -5;
    while (info->tokens[i].str && info->tokens[i].type[0] != 'p')
    {
        if (info->tokens[i].type[0] == 'g')
        {
            int a;
            a = write(fd, "\0", 1);
            if (a == -1 && fd != -5)
                opening_error(info->tokens[i].args[0]);
            close(fd);
            fd = open(info->tokens[i].args[0], O_CREAT | O_WRONLY | O_TRUNC, 0777);
            a = write(fd, "\0", 1);
            if (a == -1)
                opening_error(info->tokens[i].args[0]);
            flag = 1;
        }
        if (info->tokens[i].type[0] == 'G')
        {
            int a;
            a = write(fd, "\0", 1);
            if (a == -1 && fd != -5)
                opening_error(info->tokens[i].args[0]);
            close(fd);
            fd = open(info->tokens[i].args[0], O_CREAT | O_WRONLY | O_APPEND, 0777);
            a = write(fd, "\0", 1);
            if (a == -1)
                opening_error(info->tokens[i].args[0]);
            flag = 1;
        }
        i++;
    }
    if (!flag)
        return;
    dup2(fd, 1);
    close(fd);
}


int define_fd_in(t_info *info)
{
    int i;
    int fd;
    int flag;

    flag = 0;
    i = info->i2;
    fd = 0;
    while (info->tokens[i].str && info->tokens[i].type[0] != 'p')
    {
        if (info->tokens[i].type[0] == 'l')
        {
            if (fd)
                close(fd);
            fd = open(info->tokens[i].args[0], O_RDONLY);
            if (fd == -1)
                opening_error(info->tokens[i].args[0]);
        }
        if (info->tokens[i].type[0] == 'L')
        {
            if (fd)
                close(fd);
            fd = open("heredoc.tmp", O_CREAT | O_RDWR | O_TRUNC, 0777);
            char *buf;
            char *str;
            int len;
            buf = malloc(sizeof(char) * 100);
            str = heredoc_str(info->tokens[i].args[0], buf, &len);
            while (str)
            {
                write(fd, buf, len);
                str = heredoc_str(info->tokens[i].args[0], buf, &len);
            }
            free(buf);
            close(fd);
            fd = open("heredoc.tmp", O_RDONLY);
        }
        i++;
    }
    dup2(fd, 0);
    return (fd);
}

int define_fd_built_in(t_info *info)
{
    int i;
    int fd;
    int a;

    i = info->i2;
    fd = 0;
    while (info->tokens[i].str && info->tokens[i].type[0] != 'p')
    {
        if (info->tokens[i].type[0] == 'g')
        {
            write (fd, "\0", 1);
            if (fd)
                close(fd);
            fd = open(info->tokens[i].args[0], O_CREAT | O_WRONLY | O_TRUNC, 0777);
            a = write (fd, "\0", 1);
            if (a != 1 && fd != -5)
                opening_error(info->tokens[i].args[0]);
        }
        if (info->tokens[i].type[0] == 'G')
        {
            write(fd, "\0", 1);
            if (fd)
                close(fd);
            fd = open(info->tokens[i].args[0], O_CREAT | O_WRONLY | O_APPEND, 0777);
            a = write (fd, "\0", 1);
            if (a != 1 && fd != -5)
                opening_error(info->tokens[i].args[0]);
        }
        i++;
    }
    // if (fd == -1)
    //     return (1);
    return (fd);
}
int count_files(t_info *info, int q)
{
    int files;
    int q2;
    int i = info->i2;

    files = 0;
    q2 = 0;
    while (info->tokens[i].str && info->tokens[i].type[0] != 'p')
    {
        if (info->tokens[i].type[0] == 'l')
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

int count_redir(t_info *info)
{
    int i;
    int smb;

    i = info->i2;
    smb = 0;
    while (info->tokens[i].str && info->tokens[i].type[0] != 'p')
    {
        if (info->tokens[i].type[0] == 'l')
            smb++;
        i++;
    }
    return (smb);
}

void opening_error(char *filename)
{
    write(2, "-dashBash: ", 11);
    write(2, filename, ft_strlen(filename));
    if (errno == 2)
        write(2, ": No such file or directory\n", 28);
    else if (errno == 13 || errno == 9)
        write(2, ": Permission denied\n", 21);
    else if (errno == 20)
        write(2, ": Not a directory\n", 18);
    else if (errno == 2)
        write(2, ": No such file or directory\n", 28);
    exit(1);
}