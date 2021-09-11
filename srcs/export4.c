#include "../minishell.h"

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

void one_equality(char *str, char **new, int i)
{
    int a;
    int b;

    a = 0;
    b = 0;
    while (str[a])
    {
        (*new)[b] = str[a];
        if (str[a] == '=')
        {
            a += i - 1;
            i = 0;
        }
        b++;
        a++;
    }
    (*new)[b] = '\0';
}

char *remove_eqs(char *str, char *ptr_to_eq)
{
    int i;
    char *new;

    i = 0;
    if (ptr_to_eq[i + 1] != '=')
        return str;
    while (ptr_to_eq[i] && ptr_to_eq[i] == '=')
        i++;
    new = malloc(sizeof(char) * (ft_strlen(str) - i + 2));
    one_equality(str, &new, i);
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