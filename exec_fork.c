#include "minishell.h"

void first_fork(t_info **info, char *cmd, int ***fd, int *k)
{
    int new_fd;

    signal(SIGQUIT, SIG_DFL);
    if ((*info)->pipes_num != 0)
    {
        dup2((*fd)[*k][1], STDOUT_FILENO);
        close((*fd)[*k][0]);
        close((*fd)[*k][1]);
    }
    exec_printable(*info, cmd);
    if ((*info)->result)
    {
        new_fd = define_fd_built_in(*info);
        write(new_fd, (*info)->result, ft_strlen((*info)->result));
        free((*info)->result);
        (*info)->result = NULL;
    }
    exit(0);
}

void between_fork(t_info **info, char *cmd, int ***fd, int *k)
{
    int new_fd;
    int j;

    dup2((*fd)[*k - 1][0], STDIN_FILENO);
    dup2((*fd)[*k][1], STDOUT_FILENO);
    j = 0;
    while (j <= *k)
    {
        close((*fd)[j][0]);
        close((*fd)[j][1]);
        j++;
    }
    (*info)->i2 = (*info)->i;
    exec_printable((*info), cmd);
    if ((*info)->result)
    {
        new_fd = define_fd_built_in(*info);
        write(new_fd, (*info)->result, ft_strlen((*info)->result));
        free((*info)->result);
        (*info)->result = NULL;
    }
    exit(0);
}

void last_fork(t_info **info, char *cmd, int ***fd, int *k)
{
    int j;
    int new_fd;

    dup2((*fd)[*k - 1][0], STDIN_FILENO);
    j = 0;
    while (j <= *k - 1)
    {
        close((*fd)[j][0]);
        close((*fd)[j][1]);
        j++;
    }
    (*info)->i2 = (*info)->i;
    exec_printable(*info, cmd);
    if ((*info)->result)
    {
        new_fd = define_fd_built_in(*info);
        write(new_fd, (*info)->result, ft_strlen((*info)->result));
        free((*info)->result);
        (*info)->result = NULL;
    }
    exit(0);
}

void exec_fork(t_info **info, int ***fd, int **pids, int *k)
{
    int j;
    char *cmd;

    j = 0;
    cmd = find_bin(*info);
    (*pids)[*k] = fork();
    g_global.f = 1;
    if (*k == 0 && (*pids)[*k] == 0)
        first_fork(info, cmd, fd, k);
    else if (*k != (*info)->pipes_num && (*pids)[*k] == 0)
        between_fork(info, cmd, fd, k);
    else if (*k == (*info)->pipes_num && (*pids)[*k] == 0)
        last_fork(info, cmd, fd, k);
    free(cmd);
}
