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

// void print_exp_vars(t_info *info)
// {
    
// }

void export(t_info *info)
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
