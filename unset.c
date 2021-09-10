#include "minishell.h"

void compare_vars(t_list *tmp, t_info *info, t_list **list, t_list **prev)
{
    if (tmp == *list)
    {
        *list = tmp->next;
        free(tmp->content);
        free(tmp);
        return;
    }
    else
    {
        (*prev)->next = tmp->next;
        ft_lstdelone(tmp, free);
        return;
    }
}

void remove_var(t_info *info, t_list **list, int a)
{
    t_list *tmp;
    int var_len;
    t_list *prev;

    tmp = *list;
    var_len = ft_strlen(info->tokens[info->i].args[a]);
    while (tmp)
    {
        if (!ft_strncmp(tmp->content, info->tokens[info->i].args[a], var_len) && tmp->content[var_len] == '=')
            compare_vars(tmp, info, list, &prev);
        prev = tmp;
        tmp = tmp->next;
    }
}

int check_unset_var(t_info *info, int a)
{
    int i;

    i = 0;
    while (info->tokens[info->i].args[a][i])
    {
        if (!ft_isalpha(info->tokens[info->i].args[a][i]) && !ft_isdigit(info->tokens[info->i].args[a][i]))
        {
            write(2, "-dashBash: unset: `", 19);
            write(2, info->tokens[info->i].args[a], ft_strlen(info->tokens[info->i].args[a]));
            write(2, "': not a valid identifier\n", 26);
            return (0);
        }
        i++;
    }
    return (1);
}

void unset(t_info *info)
{
    int a;

    a = 1;
    while (info->tokens[info->i].args[a])
    {
        if (!check_unset_var(info, a))
            return;
        remove_var(info, &info->head, a);
        remove_from_extra_exp(&info->extra_exp, info->tokens[info->i].args[a]);
        if ((!ft_strncmp(info->tokens[info->i].args[a], "PATH", 4) && ft_strlen(info->tokens[info->i].args[a]) == 4) || (!ft_strncmp(info->tokens[info->i].args[a], "PWD", 3) && ft_strlen(info->tokens[info->i].args[a]) == 3) || (!ft_strncmp(info->tokens[info->i].args[a], "OLDPWD", 6) && ft_strlen(info->tokens[info->i].args[a]) == 6))
        {
            set_pointers(info);
            make_paths(info);
        }
        a++;
    }
}