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

    // if ((!ft_strncmp("echo", info->tokens[info->i].str, 4) && ft_strlen(info->tokens[info->i].str) == 4)
    // || (!ft_strncmp("export", info->tokens[info->i].str, 6) && ft_strlen(info->tokens[info->i].str) == 6)
    // || (!ft_strncmp("env", info->tokens[info->i].str, 3) && ft_strlen(info->tokens[info->i].str) == 3))
    //     return (NULL);
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
    i = info->i;
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
    i = info->index;
    fd = -5;
    while (info->tokens[i].str && info->tokens[i].type[0] != 'p')
    {
        if (info->tokens[i].type[0] == 'l')
        {
            close(fd);
            fd = open(info->tokens[i].args[0], O_RDONLY);
            if (fd == -1)
                opening_error(info->tokens[i].args[0]);
            else
                flag = 1;
        }
        if (info->tokens[i].type[0] == 'L')
        {
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
            flag = 1;
        }
        i++;
    }
    if (!flag)
        return (0);
    dup2(fd, 0);
    return (fd);
}

int define_fd_built_in(t_info *info)
{
    int i;
    int fd;
    int a;

    i = info->i;
    fd = -1;
    while (info->tokens[i].str && info->tokens[i].type[0] != 'p')
    {
        if (info->tokens[i].type[0] == 'g')
        {
            write (fd, "\0", 1);
            close(fd);
            fd = open(info->tokens[i].args[0], O_CREAT | O_WRONLY | O_TRUNC, 0777);
            a = write (fd, "\0", 1);
            if (a != 1 && fd != -5)
                opening_error(info->tokens[i].args[0]);
        }
        if (info->tokens[i].type[0] == 'G')
        {
            write(fd, "\0", 1);
            close(fd);
            fd = open(info->tokens[i].args[0], O_CREAT | O_WRONLY | O_APPEND, 0777);
            a = write (fd, "\0", 1);
            if (a != 1 && fd != -5)
                opening_error(info->tokens[i].args[0]);
        }
        i++;
    }
    if (fd == -1)
        return (1);
    return (fd);
}
int count_files(t_info *info)
{
    int files;

    files = 0;
    while (info->tokens[info->i2].str && info->tokens[info->i2].type[0] != 'p')
    {
        if (info->tokens[info->i2].type[0] == 'l')
        {
            while (info->tokens[info->i2].args[files])
                files++;
            return (files);
        }
        info->i2++;
    }
    return (0);
}
int open_file_in(t_info *info, int a)
{
    int fd;

    while (info->tokens[info->i2].str && info->tokens[info->i2].type[0] != 'p')
    {
        if (info->tokens[info->i2].type[0] == 'l')
        {
            fd = open(info->tokens[info->i2].args[a], O_RDONLY);
            if (fd == -1)
                opening_error(info->tokens[info->i2].args[a]);
            dup2(fd, 0);
            return fd;
        }
        info->i2++;
    }
    return 0;
}

int count_redir(t_info *info)
{
    int i;
    int smb;

    i = info->index;
    smb = 0;
    while (info->tokens[i].str && info->tokens[i].type[0] != 'p')
    {
        if (info->tokens[i].type[0] == 'l')
            smb++;
        i++;
    }
    return (smb);
}

void exec_once(t_info *info, char *cmd)
{
    int fd;

    define_fd_out(info);
    fd = define_fd_in(info);
    if (fd != -1)
        execve(cmd, info->tokens[info->i].args, 0);
}

void exec_few_times(int *flag, t_info *info, char *cmd, int files)
{
    int a = 0;
    int fd;
    pid_t pid;

    *flag = 1;
    fd = open_file_in(info, a);
    if (fd == -1)
        return;
    while (a < files - 1)
    {
        a++;
        define_fd_out(info);
        fd = open_file_in(info, a);
        pid = fork();
	    if (pid == 0)
        {
            if (fd != -1)
		        execve(cmd, info->tokens[info->i].args, 0);
            exit(0);
        }
        else
        {
        	waitpid(pid, 0, 0);
            close(fd);
        }
    }
}

void replace_index(t_info *info)
{
    while (info->tokens[info->index].str && info->tokens[info->index].type[0] != 'p')
        info->index++;
    if (info->tokens[info->index].str)
        info->index++;
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