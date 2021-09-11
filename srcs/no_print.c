#include "../minishell.h"

void exec_builtin(t_info *info)
{
    if (!ft_strncmp(info->tokens[info->i].str, "cd", 2) && ft_strlen(info->tokens[info->i].str) == 2)
        cd(info);
    else if (ft_strlen(info->tokens[info->i].str) == 5 && !ft_strncmp(info->tokens[info->i].str, "unset", 5))
        unset(info);
    else if (ft_strlen(info->tokens[info->i].str) == 4 && !ft_strncmp(info->tokens[info->i].str, "exit", 4))
        exit_minishell(info);
    else if (ft_strlen(info->tokens[info->i].str) == 6 && !ft_strncmp(info->tokens[info->i].str, "export", 6))
        export(info);
    g_global.ex_status = 0;
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
