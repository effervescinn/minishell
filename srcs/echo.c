#include "../minishell.h"

void echo_print(t_info *info, int a, int n)
{
    while (info->tokens[info->i].args[a])
    {
        info->result = no_leaks_join(info->result, info->tokens[info->i].args[a]);
        if (info->tokens[info->i].args[a + 1])
            info->result = no_leaks_join(info->result, " ");
        a++;
    }
    if (!n)
        info->result = no_leaks_join(info->result, "\n");
}


void echo(t_info *info)
{
    int n;
    int a;

    n = 0;
    a = 1;
    if (!info->tokens[info->i].args[a])
    {
        info->result = ft_strdup("\n\0");
        return;
    }
    if (ft_strlen(info->tokens[info->i].args[a]) == 2 && !ft_strncmp(info->tokens[info->i].args[a], "-n", 2))
    {
        a++;
        if (!info->tokens[info->i].args[a])
        {
            info->result = ft_strdup("\0");
            return;
        }
        n = 1;
    }
    echo_print(info, a, n);
}