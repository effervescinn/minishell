#include "minishell.h"

int check_redirs(t_info **info, int ***fd, int *k)
{
    if (*k < (*info)->pipes_num)
        pipe((*fd)[*k]);
    if ((*info)->tokens[(*info)->i].type != 'c'
    && (*info)->tokens[(*info)->i].type != 'L')
    {
        if ((*info)->tokens[(*info)->i + 2].str
        && (*info)->tokens[(*info)->i + 2].type == 'c')
            (*info)->i += 2;
        else
        {
            redirects_solo(*info);
            if ((*info)->tokens[(*info)->i].str
            && (*info)->tokens[(*info)->i + 1].str)
                (*info)->i += 1;
            return (1);
        }
    }
    return (0);
}

void init_pids(t_info **info, int **pids, int ***fd)
{
    int j;

    j = 0;
    *pids = (int *)malloc(sizeof(int) * ((*info)->pipes_num + 1));
    *fd = (int **)malloc(sizeof(int *) * (*info)->pipes_num);
    j = 0;
    while (j < (*info)->pipes_num)
    {
        (*fd)[j] = (int *)malloc(sizeof(int) * 2);
        j++;
    }
    (*info)->result = ft_strdup("\0");
    search_heredoc(*info);
}

int no_print(t_info **info, int ***fd, int *k)
{
    if (*k == 0 && (*info)->pipes_num == 0)
        exec_builtin(*info);
    else
    {
        ((*info)->i)++;
        while ((*info)->tokens[(*info)->i].str
        && (*info)->tokens[(*info)->i].type != 'p')
            ((*info)->i)++;
        ((*info)->i)++;
        if (*k < (*info)->pipes_num)
            write((*fd)[*k][1], "\0", 1);
        return (1);
    }
    return (0);
}

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

void pd_utils(t_info **info, int ***fd)
{
    int k;
    int ex;

    k = 0;
    while (k < (*info)->pipes_num)
    {
        close((*fd)[k][0]);
        close((*fd)[k][1]);
        k++;
    }
    k = 0;
    while (k < (*info)->pipes_num + 1)
    {
        waitpid((*info)->pids[k], &ex, 0);
        k++;
    }
    g_global.ex_status = WEXITSTATUS(ex);
    g_global.f = 0;
    unlink_files(*info);
}

void free_pids(t_info **info, int ***fd)
{
    int j;

    if ((*info)->result)
    {
        free((*info)->result);
        (*info)->result = NULL;
    }
    ((*info)->i) = 0;
    free((*info)->pids);
    j = 0;
    while (j < (*info)->pipes_num)
    {
        free((*fd)[j]);
        j++;
    }
    free(*fd);
}

void program_define(t_info *info)
{
    int **fd;
    int k;

    init_pids(&info, &info->pids, &fd);
    k = -1;
    while (++k < info->pipes_num + 1)
    {
        if (check_redirs(&info, &fd, &k))
            continue;
        if (info->tokens[info->i].print == 0)
        {
            if (no_print(&info, &fd, &k))
                continue;
        }
        else
            exec_fork(&info, &fd, &info->pids, &k);
        while (info->tokens[info->i].str && info->tokens[info->i].type != 'p')
            (info->i)++;
        (info->i)++;
    }
    pd_utils(&info, &fd);
    free_pids(&info, &fd);
}
