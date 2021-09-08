#include "minishell.h"

void print_exp_vars(t_info *info)
{
    t_list *tmp;

    make_exp(info);
    tmp = info->exp;
    while (tmp)
    {
        if (ft_strchr(tmp->content, '='))
            tmp->content = add_quotes(tmp->content);
        tmp = tmp->next;
    }
    tmp = info->exp;
    while (tmp)
    {
        info->result = no_leaks_join(info->result, "declare -x ");
        info->result = no_leaks_join(info->result, tmp->content);
        info->result = no_leaks_join(info->result, "\n");
        tmp = tmp->next;
    }
    free_list(&info->exp);
}

void no_quotes(char *str)
{
    char *new_string;
    int i;
    int k;
    int quotes;

    i = 0;
    quotes = 0;
    while (str[i])
    {
        if (str[i] == 34 || str[i] == 39)
            quotes++;
        i++;
    }
    if (quotes == 0)
        return;
    new_string = malloc(sizeof(char) * (ft_strlen(str) - quotes + 1));
    i = 0;
    k = 0;
    while (str[i])
    {
        if (str[i] != 34 && str[i] != 39)
        {
            new_string[k] = str[i];
            i++;
        }
        k++;
    }
    new_string[k] = '\0';
    free(str);
    str = new_string;
}

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

char *var_name_in_str(char *str, char *ptr_to_eq)
{
    char *var_name;
    int var_len;
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
            if (tmp == *list)
            {
                *list = tmp->next;
                free(tmp->content);
                free(tmp);
                return;
            }
            else
            {
                prev->next = tmp->next;
                ft_lstdelone(tmp, free);
            }
        }
        prev = tmp;
        tmp = tmp->next;
    }
}

char *remove_plus(char *s)
{
    char *new;
    char *ptr_to_plus;
    int i;
    int j;

    ptr_to_plus = ft_strchr(s, '+');
    if (!ptr_to_plus)
        return s;
    new = malloc(sizeof(char) * ft_strlen(s));
    i = 0;
    j = 0;
    while (s[i])
    {
        if (s + i == ptr_to_plus)
            i++;
        new[j++] = s[i++];
    }
    new[j] = '\0';
    free(s);
    return (new);
}

void find_and_join(char *ptr_to_eq, t_info *info, char *var, int a)
{
    t_list *tmp;
    int var_len;
    t_list *new;

    tmp = info->head;
    var_len = ft_strlen(var);
    while (tmp)
    {
        if (!ft_strncmp(tmp->content, var, var_len))
        {
            tmp->content = no_leaks_join(tmp->content, ptr_to_eq + 1);
            return;
        }
        tmp = tmp->next;
    }
    info->tokens[info->i].args[a] = remove_plus(info->tokens[info->i].args[a]);
    new = malloc(sizeof(t_list));
    new->content = ft_strdup(info->tokens[info->i].args[a]);
    ft_lstadd_back(&info->head, new);
}

char *remove_eqs(char *str, char *ptr_to_eq)
{
    int a;
    int b;
    int i;
    char *new;

    i = 0;
    while (ptr_to_eq[i] && ptr_to_eq[i] == '=')
        i++;
    new = malloc(sizeof(char) * (ft_strlen(str) - i + 2));
    a = 0;
    b = 0;
    while (str[a])
    {
        new[b] = str[a];
        if (str[a] == '=')
        {
            a += i - 1;
            i = 0;
        }
        b++;
        a++;
    }
    new[b] = '\0';
    free(str);
    return (new);
}

int check_var_name(char *var_name)
{
    int i;

    i = 0;
    while (var_name[i])
    {
        if (!ft_isdigit(var_name[i]) && !ft_isalpha(var_name[i]) && !(var_name[i] == '+' && var_name[i + 1] == '\0'))
            return (0);
        i++;
    }
    return (1);
}

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

void export(t_info *info)
{
    char *ptr_to_eq;
    char *ptr_to_space;
    char *var_name = 0;
    int a;

    a = 1;
    if (!info->tokens[info->i].args[1])
        print_exp_vars(info);
    else
    {
        while (info->tokens[info->i].args[a])
        {

            no_quotes(info->tokens[info->i].args[a]);
            ptr_to_eq = ft_strchr(info->tokens[info->i].args[a], '=');
            if ((!ft_strncmp(info->tokens[info->i].args[a], "PWD\0", 4) && ft_strlen(info->tokens[info->i].args[a]) == 3 && !info->pwd) ||
                (!ft_strncmp(info->tokens[info->i].args[a], "OLDPWD\0", 7) && ft_strlen(info->tokens[info->i].args[a]) == 6 && !info->oldpwd))
            {
                find_and_join(ptr_to_eq, info, info->tokens[info->i].args[a], a);
                set_pointers(info);
                a++;
                continue;
            }
            if ((!ft_strncmp(info->tokens[info->i].args[a], "PWD\0", 4) && ft_strlen(info->tokens[info->i].args[a]) == 3 && info->pwd) ||
                (!ft_strncmp(info->tokens[info->i].args[a], "OLDPWD\0", 7) && ft_strlen(info->tokens[info->i].args[a]) == 6 && info->oldpwd))
            {
                a++;
                continue;
            }
            if (ptr_to_eq && ptr_to_eq != info->tokens[info->i].args[a])
                info->tokens[info->i].args[a] = remove_eqs(info->tokens[info->i].args[a], ptr_to_eq);
            ptr_to_eq = ft_strchr(info->tokens[info->i].args[a], '=');
            if (ptr_to_eq && ptr_to_eq != info->tokens[info->i].args[a])
                var_name = var_name_in_str(info->tokens[info->i].args[a], ptr_to_eq);
            else
                var_name = ft_strdup(info->tokens[info->i].args[a]);
            if (!check_var_name(var_name))
            {
                write(2, "-dashBash: export: `", 20);
                write(2, info->tokens[info->i].args[a], ft_strlen(info->tokens[info->i].args[a]));
                write(2, "': not a valid identifier\n", 27);
                a++;
                continue;
            }
            if (ptr_to_eq && *(ptr_to_eq - 1) == '+')
            {
                find_and_join(ptr_to_eq, info, var_name, a);
                remove_from_extra_exp(&info->extra_exp, var_name);
                set_pointers(info);
                make_paths(info);
                free(var_name);
                return;
            }
            if (ptr_to_eq)
            {
                find_existing_var(var_name, info, a);
                remove_from_extra_exp(&info->extra_exp, var_name);
                set_pointers(info);
            }
            else
            {
                if (!check_env_vars(info, a))
                    extra_export(info, a);
            }
            free(var_name);
            a++;
        }
    }
}