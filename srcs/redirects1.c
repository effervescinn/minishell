#include "../minishell.h"

void g_redirects_solo(t_info *info, int i)
{
    int fd;
    int a;

    if (info->tokens[i].type == 'g')
    {
        fd = open(info->tokens[i].args[0], O_CREAT | O_WRONLY | O_TRUNC, 0777);
        a = write(fd, NULL, 0);
        if (a == -1)
            opening_error(info->tokens[i].args[0]);
        close(fd);
    }
    if (info->tokens[i].type == 'G')
    {
        fd = open(info->tokens[i].args[0], O_CREAT | O_WRONLY | O_APPEND, 0777);
        a = write(fd, NULL, 0);
        if (a == -1)
            opening_error(info->tokens[i].args[0]);
        close(fd);
    }
}
void redirects_solo(t_info *info)
{
    int i;
    int fd;

    i = info->i;
    while (info->tokens[i].str && info->tokens[i].type != 'p')
    {
        g_redirects_solo(info, i);
        if (info->tokens[i].type == 'l')
        {
            fd = open(info->tokens[i].args[0], O_RDONLY);
            if (fd == -1)
                opening_error(info->tokens[i].args[0]);
            close(fd);
        }
        i++;
    }
    info->i = i;
}

void found_g_redirect(int *fd, t_info *info, int i, int *flag)
{
    int a;

    if (info->tokens[i].type == 'g')
    {
        if (*fd)
            close(*fd);
        *fd = open(info->tokens[i].args[0], O_CREAT | O_WRONLY | O_TRUNC, 0777);
        a = write(*fd, NULL, 0);
        if (a == -1)
            opening_error(info->tokens[i].args[0]);
        *flag = 1;
    }
    if (info->tokens[i].type == 'G')
    {
        if (*fd)
            close(*fd);
        *fd = open(info->tokens[i].args[0], O_CREAT | O_WRONLY | O_APPEND, 0777);
        a = write(*fd, NULL, 0);
        if (a == -1)
            opening_error(info->tokens[i].args[0]);
        *flag = 1;
    }
}

void define_fd_out(t_info *info)
{
    int i;
    int fd;
    int flag;

    flag = 0;
    i = info->i2;
    fd = 0;
    while (info->tokens[i].str && info->tokens[i].type != 'p')
    {
        found_g_redirect(&fd, info, i, &flag);
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
    while (info->tokens[i].str && info->tokens[i].type != 'p')
    {
        if (info->tokens[i].type == 'l')
        {
            if (fd)
                close(fd);
            fd = open(info->tokens[i].args[0], O_RDONLY);
            if (fd == -1)
                opening_error(info->tokens[i].args[0]);
        }
        i++;
    }
    dup2(fd, 0);
    return (fd);
}