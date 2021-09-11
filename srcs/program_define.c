#include "../minishell.h"

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
