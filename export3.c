#include "minishell.h"

void find_existing_var(char *var_name, t_info *info, int a)
{
    t_list *tmp;
    int var_len;
    char *tmp2;

    tmp = info->head;
    var_len = ft_strlen(var_name);
    while (tmp)
    {
        if (!ft_strncmp(tmp->content, var_name, var_len))
        {
            if (tmp->content[var_len] == '=' || tmp->content[var_len] == '\0')
            {
                tmp2 = tmp->content;
                tmp->content = ft_strdup(info->tokens[info->i].args[a]);
                free(tmp2);
                return;
            }
        }
        tmp = tmp->next;
    }
    tmp = malloc(sizeof(t_list));
    tmp->content = ft_strdup(info->tokens[info->i].args[a]);
    ft_lstadd_back(&info->head, tmp);
}

char *var_name_in_str(char *str)
{
    char *var_name;
    int i;
    i = 0;

    while (str[i] && str[i] != '=')
        i++;
    if (str[i - 1] == '+')
        i--;
    var_name = malloc(sizeof(char) * (i + 1));
    ft_memcpy(var_name, str, i);
    var_name[i] = '\0';
    return (var_name);
}

void extra_export(t_info *info, int a)
{
    t_list *new;

    if (!info->extra_exp)
        info->extra_exp = ft_lstnew(ft_strdup(info->tokens[info->i].args[a]));
    else
    {
        new = malloc(sizeof(t_list));
        new->content = ft_strdup(info->tokens[info->i].args[a]);
        ft_lstadd_back(&info->extra_exp, new);
    }
}

void remove_extra(t_list **tmp, t_list **list, t_list **prev)
{
    if (*tmp == *list)
    {
        *list = (*tmp)->next;
        free((*tmp)->content);
        free(*tmp);
        return;
    }
    else
    {
        (*prev)->next = (*tmp)->next;
        ft_lstdelone(*tmp, free);
        *tmp = NULL;
    }
}

void remove_from_extra_exp(t_list **list, char *var)
{
    t_list *tmp;
    int var_len;
    t_list *prev;

    tmp = *list;
    var_len = ft_strlen(var);
    while (tmp)
    {
        if (!ft_strncmp(tmp->content, var, var_len) && tmp->content[var_len] == '\0')
        {
            remove_extra(&tmp, list, &prev);
            return;
        }
        prev = tmp;
        if (tmp->next)
            tmp = tmp->next;
    }
}