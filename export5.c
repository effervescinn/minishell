#include "minishell.h"

int check_env_vars(t_info *info, int a)
{
    t_list *tmp;
    int var_len;

    tmp = info->head;
    var_len = ft_strlen(info->tokens[info->i].args[a]);
    while (tmp)
    {
        if (!ft_strncmp(tmp->content, info->tokens[info->i].args[a], var_len) && tmp->content[var_len] == '=')
            return (1);
        tmp = tmp->next;
    }
    return (0);
}

void var_name_error(int *a, char **var_name, t_info *info)
{
    write(2, "-dashBash: export: `", 20);
    write(2, info->tokens[info->i].args[*a], ft_strlen(info->tokens[info->i].args[*a]));
    write(2, "': not a valid identifier\n", 27);
    (*a)++;
    free(*var_name);
}

int plus_before_eq(char **ptr_to_eq, t_info *info, char **var_name, int a)
{
    if (*ptr_to_eq && *(*ptr_to_eq - 1) == '+')
    {
        find_and_join(*ptr_to_eq, info, *var_name, a);
        remove_from_extra_exp(&info->extra_exp, *var_name);
        set_pointers(info);
        make_paths(info);
        free(*var_name);
        return (1);
    }
    return (0);
}

int export_end(char **ptr_to_eq, t_info *info, char **var_name, int *a)
{
    if (plus_before_eq(ptr_to_eq, info, var_name, *a))
    {
        (*a)++;
        return (1);
    }
    if (*ptr_to_eq)
    {
        find_existing_var(*var_name, info, *a);
        remove_from_extra_exp(&info->extra_exp, *var_name);
        set_pointers(info);
        make_paths(info);
    }
    else
    {
        if (!check_env_vars(info, *a))
            extra_export(info, *a);
    }
    free(*var_name);
    (*a)++;
    return (0);
}

int export_if_pwd(t_info *info, int *a, char **ptr_to_eq)
{
    if ((!ft_strncmp(info->tokens[info->i].args[*a], "PWD\0", 4) && ft_strlen(info->tokens[info->i].args[*a]) == 3 && !info->pwd) ||
            (!ft_strncmp(info->tokens[info->i].args[*a], "OLDPWD\0", 7) && ft_strlen(info->tokens[info->i].args[*a]) == 6 && !info->oldpwd))
    {
        find_and_join(*ptr_to_eq, info, info->tokens[info->i].args[*a], *a);
        set_pointers(info);
        (*a)++;
        return (1);
    }
    if ((!ft_strncmp(info->tokens[info->i].args[*a], "PWD\0", 4) && ft_strlen(info->tokens[info->i].args[*a]) == 3 && info->pwd) ||
            (!ft_strncmp(info->tokens[info->i].args[*a], "OLDPWD\0", 7) && ft_strlen(info->tokens[info->i].args[*a]) == 6 && info->oldpwd))
        {
            (*a)++;
            return(1);
        }
    return (0);
}