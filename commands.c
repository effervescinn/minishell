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
    exp_tmp->next = NULL;
    export_order(info, i);
    write(1, "********************************************\n", 46);
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
    if (ft_strlen(info->tokens[info->i].str) == 2 && !ft_strncmp(info->tokens[info->i].str, "-n", 2))
    {
		info->i++;
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

void print_exp_vars(t_info *info)
{
    t_list *tmp;

    tmp = info->exp;
    while (tmp->next)
    {
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

void find_existing_var(t_list **list, char *var_name, char *new_str, t_info *info)
{
    t_list *tmp;
    int var_len;
    char *tmp2;

    tmp = *list;
    var_len = ft_strlen(var_name);
    while (tmp->next)
    {
        if (!ft_strncmp(tmp->content, var_name, var_len))
        {
            if (tmp->content[var_len] == '=' || tmp->content[var_len] == '\0')
            {
                tmp2 = tmp->content;
                tmp->content = new_str;
                // free(tmp2);
                make_exp(info);
                return;
            }
        }
        tmp = tmp->next;
    }
    tmp = malloc(sizeof(t_list));
    tmp->content = new_str;
    ft_lstadd_back(list, tmp);
}

char *var_name_in_str(char *str, char *ptr_to_eq)
{
    char *var_name;
    int var_len;

    ptr_to_eq = ft_strchr(str, '=');
    var_len = ptr_to_eq - str;
    var_name = malloc(sizeof(char) * (var_len + 1));
    ft_memcpy(var_name, str, var_len);
    var_name[var_len] = '\0';
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
        ptr_to_eq = ft_strchr(info->tokens[info->i].str, '=');
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
            printf("replase por %s***********************************************\n", info->tokens[info->i].str);
            find_existing_var(&info->head, var_name, info->tokens[info->i].str, info);

        }
        else
        printf("*\n");

    }
        // printf("*\n");
}

// void free_tokens(t_info *info)
// {
//     int i = 0;
//     while (info->tokens[i].str)
//     {
//         free(info->tokens[i].str);
//         // free(info->tokens[i].type);
//         i++;
//     }
//     free(info->tokens);
// }

void program_define(t_info *info)
{
    if (ft_strlen(info->tokens[info->i].str) == 3 && !ft_strncmp(info->tokens[info->i].str, "pwd", 3))
		pwd(info);
    else if (!ft_strncmp(info->tokens[info->i].str, "cd", 2) && ft_strlen(info->tokens[info->i].str) == 2)
    {
		info->i++;
        chdir(info->tokens[info->i].str);
    }
    else if (ft_strlen(info->tokens[info->i].str) == 4 && !ft_strncmp(info->tokens[info->i].str, "echo", 4))
		echo(info);
	else if (ft_strlen(info->tokens[info->i].str) == 3 && !ft_strncmp(info->tokens[info->i].str, "env", 3))
		env(info);
	else if (ft_strlen(info->tokens[info->i].str) == 6 && !ft_strncmp(info->tokens[info->i].str, "export", 3))
		export(info);
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
