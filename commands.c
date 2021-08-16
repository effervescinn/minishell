#include "minishell.h"

void set_pointers(t_info *info)
{
    t_list *tmp;

    tmp = info->head;
    while (tmp)
    {
        if (!ft_strncmp(tmp->content, "OLDPWD=", 7))
        {
            info->oldpwd = tmp;
        }
        if (!ft_strncmp(tmp->content, "PWD=", 4))
            info->pwd = tmp;
        else if (!ft_strncmp(tmp->content, "PATH=", 5))
            info->pths = tmp;
        tmp = tmp->next;
    }
}

void swap_content(t_list *list1, t_list *list2)
{
    char *tmp;

    tmp = list1->content;
    list1->content = list2->content;
    list2->content = tmp;
}

void export_order(t_info *info, int i)
{
    t_list *next;
    t_list *tmp;
    int dif;
    t_list *head;

    head = info->exp;
    while (i--)
    {
        while (info->exp->next)
        {
            next = info->exp->next;
            dif = ft_strncmp(info->exp->content, next->content, ft_strlen(info->exp->content));
            if (dif > 0)
                swap_content(info->exp, next);
            info->exp = info->exp->next;
        }
        info->exp = head;
    }
}

void make_exp(t_info *info)
{
    t_list *env_tmp;
    t_list *exp_tmp;
    t_list *extra_tmp;
    int i;

    i = 1;
    env_tmp = info->head;
    info->exp = ft_lstnew(ft_strdup(info->head->content));
    exp_tmp = info->exp;
    while (env_tmp->next)
    {
        env_tmp = env_tmp->next;
        exp_tmp->next = malloc(sizeof(t_list));
        exp_tmp = exp_tmp->next;
        exp_tmp->content = ft_strdup(env_tmp->content);
        i++;
    }
    if (!info->extra_exp)
        exp_tmp->next = NULL;
    else
    {
        extra_tmp = info->extra_exp;
        while (extra_tmp)
        {
            exp_tmp->next = malloc(sizeof(t_list));
            exp_tmp = exp_tmp->next;
            exp_tmp->content = ft_strdup(extra_tmp->content);
            extra_tmp = extra_tmp->next;
            i++;
        }
        exp_tmp->next = NULL;
    }
    export_order(info, i);
}

char *no_leaks_join(char *str1, char *str2)
{
	char *tmp;

	tmp = str1;
	str1 = ft_strjoin(str1, str2);
	free(tmp);
	return (str1);
}

void echo(t_info *info)
{
	int n;

	n = 0;
	info->i++;
    if (!info->tokens[info->i].str)
    {
        info->result = ft_strdup("\n\0");
        return;
    }
    if (ft_strlen(info->tokens[info->i].str) == 2 && !ft_strncmp(info->tokens[info->i].str, "-n", 2))
    {
		info->i++;
        if (!info->tokens[info->i].str)
        {
            info->result = ft_strdup("\0");
            return;
        }
		n = 1;
    }
    while (ft_strlen(info->tokens[info->i].type) == 4 && !ft_strncmp(info->tokens[info->i].type, "word", 4))
    {
		info->result = no_leaks_join(info->result, info->tokens[info->i].str);
        info->i++;
		if (info->tokens[info->i].str)
		{
	        if (ft_strlen(info->tokens[info->i].type) == 4 && !ft_strncmp(info->tokens[info->i].type, "word", 4))
				info->result = no_leaks_join(info->result, " ");
		}
		else
			break;
    }
	if (!n)
		info->result = no_leaks_join(info->result, "\n");
}

void pwd(t_info *info)
{
	char *buf;

	buf = getcwd(NULL, 100);
    if (buf)
    {
		info->result = no_leaks_join(info->result, buf);
        free(buf);
		info->result = no_leaks_join(info->result, "\n");
    }
	else
		exit(0);
}

void env(t_info *info)
{
	t_list *tmp;

	tmp = info->head;
	while (tmp)
	{
		info->result = no_leaks_join(info->result, tmp->content);
		info->result = no_leaks_join(info->result, "\n");
		tmp = tmp->next;
	}
}

char *add_quotes(char *str)
{
    char *result;
    int i;

    i = 0;
    result = malloc(sizeof(char) * ft_strlen(str) + 3);
    while (str[i] && str[i] != '=')
    {
        result[i] = str[i];
        i++;
    }
    result[i] = '=';
    result[++i] = '"';
    i++;
    while (str[i - 1])
    {
        result[i] = str[i - 1];
        i++;
    }
    result[i] = '"';
    result[i + 1] = '\0';
    free(str);
    return (result);
}

void free_exp(t_info *info)
{
    t_list *tmp;

    while (info->exp)
    {
        tmp = info->exp;
        info->exp = info->exp->next;
        free(tmp->content);
        free(tmp);
    }
}

void print_exp_vars(t_info *info)
{
    t_list *tmp;

    if (info->exp)
        free_exp(info);
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
    while(str[i])
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

void find_existing_var(char *var_name, t_info *info)
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
                tmp->content = ft_strdup(info->tokens[info->i].str);
                free(tmp2);
                return;
            }
        }
        tmp = tmp->next;
    }
    tmp = malloc(sizeof(t_list));
    tmp->content = ft_strdup(info->tokens[info->i].str);
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

void print_export_error(char *str)
{
    char **array;
    int i;

    i = 0;
    array = ft_split(str, ' ');
    while (array[i])
    {
        printf("-dashBash: export: `%s': not a valid identifier", array[i]);
        i++;
    }
    i = 0;
    while(array[i++])
        free(array[i]);
    free(array);
}

void extra_export(t_info *info)
{
    t_list *new;

    if (!info->extra_exp)
        info->extra_exp = ft_lstnew(ft_strdup(info->tokens[info->i].str));
    else
    {
        new = malloc(sizeof(t_list));
        new->content = ft_strdup(info->tokens[info->i].str);
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

    new = malloc(sizeof(char) * ft_strlen(s));
    ptr_to_plus = ft_strchr(s, '+');
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

void find_and_join(char *ptr_to_eq, t_info *info, char *var)
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
    info->tokens[info->i].str = remove_plus(info->tokens[info->i].str);
    new = malloc(sizeof(t_list));
    new->content = ft_strdup(info->tokens[info->i].str);
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
        if(str[a] == '=')
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

void export(t_info *info)
{
    t_list *tmp;
    char *ptr_to_eq;
    char *ptr_to_space;
    char *var_name;

    info->i++;
    if (!info->tokens[info->i].str)
        print_exp_vars(info);
    else
    {
        while (info->tokens[info->i].str)
        {
            ptr_to_eq = ft_strchr(info->tokens[info->i].str, '=');
            if(ptr_to_eq)
            {
                info->tokens[info->i].str = remove_eqs(info->tokens[info->i].str, ptr_to_eq);
                ptr_to_eq = ft_strchr(info->tokens[info->i].str, '=');
            
                if (info->tokens[info->i].str[0] != '=')
                {
                    if (!ft_isdigit(*(ptr_to_eq - 1)) && !ft_isalpha(*(ptr_to_eq - 1)) && *(ptr_to_eq - 1) != '+')
                    {
                        printf("-dashBash: export: `%s': not a valid identifier\n", info->tokens[info->i].str);
                        return;
                    }
                    if (*(ptr_to_eq - 1) == '+')
                    {
                        var_name = var_name_in_str(info->tokens[info->i].str, ptr_to_eq);
                        find_and_join(ptr_to_eq, info, var_name);
                        remove_from_extra_exp(&info->extra_exp, var_name);
                        return;
                    }
                }
            }
        // ptr_to_space = ft_strchr(info->tokens[info->i].str, ' '); ///////////////////////проверить потом с новым парсером
        // if (ptr_to_space < ptr_to_eq)
        // {
        //     print_export_error(info->tokens[info->i].str);
        //     return;
        // }
            no_quotes(info->tokens[info->i].str);
            if (ptr_to_eq)
            {
                var_name = var_name_in_str(info->tokens[info->i].str, ptr_to_eq);
                find_existing_var(var_name, info);
                remove_from_extra_exp(&info->extra_exp, var_name);
            }
            else
                extra_export(info);
            info->i++;
        }
    }
}

void cd(t_info *info)
{
    char *tmp;
    char *buf;
    char *tmp2;
    int a;

    tmp = ft_strjoin("OLD", info->pwd->content);
	info->i++;
    a = chdir(info->tokens[info->i].str);
    if (!a)
    {
        info->oldpwd->content = ft_strdup(tmp);
	    buf = getcwd(NULL, 100);
        if (buf)
        {
            tmp2 = ft_strjoin("PWD=", buf);
            info->pwd->content = ft_strdup(tmp2);
            free(buf);
        }
	    else
		    exit(0);
    }
    else
    {
        if (errno == 20)
            printf("-dashBash: cd: %s: Not a directory\n", info->tokens[info->i].str);
        else if (errno == 2)
            printf("-dashBash: cd: %s: No such file or directory\n", info->tokens[info->i].str);
        else
            printf("zapomni chto ty sdelal\n");
        errno = 0;
        free(tmp);
    }
}

void remove_var(t_info *info, t_list **list)
{
    t_list *tmp;
    int var_len;
    t_list *prev;

    tmp = *list;
    var_len = ft_strlen(info->tokens[info->i].str);
    while (tmp)
    {
        if (!ft_strncmp(tmp->content, info->tokens[info->i].str, var_len) && tmp->content[var_len] == '=')
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
                return;
            }
        }
        prev = tmp;
        tmp = tmp->next;
    }
}

void unset(t_info *info)
{
    int i;

    info->i++;
    while (info->tokens[info->i].str)
    {
        i = 0;
        while (info->tokens[info->i].str[i])
        {
            if (!ft_isalpha(info->tokens[info->i].str[i]) && !ft_isdigit(info->tokens[info->i].str[i]))
            {
                printf("bash: unset: `%s': not a valid identifier\n", info->tokens[info->i].str);
                return;
            }
            i++;
        }
        remove_var(info, &info->head);
        remove_from_extra_exp(&info->extra_exp, info->tokens[info->i].str);
        info->i++;
    }
}

void program_define(t_info *info)
{
    if (ft_strlen(info->tokens[info->i].str) == 3 && !ft_strncmp(info->tokens[info->i].str, "pwd", 3))
		pwd(info);
    else if (!ft_strncmp(info->tokens[info->i].str, "cd", 2) && ft_strlen(info->tokens[info->i].str) == 2)
        cd(info);
    else if (ft_strlen(info->tokens[info->i].str) == 4 && !ft_strncmp(info->tokens[info->i].str, "echo", 4))
		echo(info);
	else if (ft_strlen(info->tokens[info->i].str) == 3 && !ft_strncmp(info->tokens[info->i].str, "env", 3))
		env(info);
	else if (ft_strlen(info->tokens[info->i].str) == 6 && !ft_strncmp(info->tokens[info->i].str, "export", 6))
		export(info);
	else if (ft_strlen(info->tokens[info->i].str) == 5 && !ft_strncmp(info->tokens[info->i].str, "unset", 5))
        unset(info);
    else
    {
        write(1, "dashBash: ", 11);
        write(1, info->tokens[info->i].str, ft_strlen(info->tokens[info->i].str));
        write(1, ": command not found\n", 21);
        info->result = NULL;
    }
    if (info->result)
    {
        printf("%s", info->result);
	    free(info->result);
    }
    info->result = malloc(1);
    info->result[0] = '\0';
	info->i = 0;
}
