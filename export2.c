#include "minishell.h"

void	make_exp(t_info *info)
{
	t_list	*env_tmp;
	t_list	*exp_tmp;
	int		i;

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
	join_env_and_extra(info, &exp_tmp, &i);
	export_order(info, i);
}

char	*add_quotes(char *str)
{
	char	*result;
	int		i;

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

void	print_exp_vars(t_info *info)
{
	t_list	*tmp;

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

void	ew_str_without_quotes(int quotes, char **str)
{
	char	*new_string;
	int		i;
	int		k;

	new_string = malloc(sizeof(char) * (ft_strlen(*str) - quotes + 1));
	i = 0;
	k = 0;
	while ((*str)[i])
	{
		if ((*str)[i] != 34 && (*str)[i] != 39)
		{
			new_string[k] = (*str)[i];
			i++;
		}
		k++;
	}
	new_string[k] = '\0';
	free(*str);
	*str = new_string;
}

void	no_quotes(char *str)
{
	int	i;
	int	quotes;

	i = 0;
	quotes = 0;
	while (str[i])
	{
		if (str[i] == 34 || str[i] == 39)
			quotes++;
		i++;
	}
	if (quotes == 0)
		return ;
	new_str_without_quotes(quotes, &str);
}
