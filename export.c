#include "minishell.h"

int export_loop(t_info *info, int *a)
{
    char *ptr_to_eq;
    char *var_name;

    no_quotes(info->tokens[info->i].args[*a]);
    ptr_to_eq = ft_strchr(info->tokens[info->i].args[*a], '=');
    if (export_if_pwd(info, a, &ptr_to_eq))
        return(1);
    if (ptr_to_eq && ptr_to_eq != info->tokens[info->i].args[*a])
        info->tokens[info->i].args[*a] = remove_eqs(info->tokens[info->i].args[*a], ptr_to_eq);
    ptr_to_eq = ft_strchr(info->tokens[info->i].args[*a], '=');
    if (ptr_to_eq && ptr_to_eq != info->tokens[info->i].args[*a])
        var_name = var_name_in_str(info->tokens[info->i].args[*a], ptr_to_eq);
    else
        var_name = ft_strdup(info->tokens[info->i].args[*a]);
    if (!check_var_name(var_name))
    {
        var_name_error(a, &var_name, info);
        return(1);
    }
    if (export_end(&ptr_to_eq, info, &var_name, a))
        return(1);
    return (0);
}

void export(t_info *info)
{
    int a;

    a = 1;
    if (!info->tokens[info->i].args[1])
        print_exp_vars(info);
    else
    {
        while (info->tokens[info->i].args[a])
        {
            if (export_loop(info, &a))
                continue;
        }
    }
}