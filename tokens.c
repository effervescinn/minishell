#include "minishell.h"

void make_env(char **envp, t_list **head)
{
	t_list *new;
	int i;

	i = 1;
	*head = ft_lstnew(ft_strdup(envp[0]));
	while (envp[i])
	{
		new = (t_list *)malloc(sizeof(t_list));
		if (ft_strncmp("_=./", envp[i], 4))
		{
			new->content = ft_strdup(envp[i]);
			new->next = NULL;
			ft_lstadd_back(head, new);
		}
		i++;
	}
}

int count_quotes(char *str)
{
	int quotes;

	quotes = 0;
	while (*str)
	{
		while (*str && *str != '\'' && *str != '\"')
			str++;
		if (*str == '\"')
		{
			str++;
			quotes++;
			str = ft_strchr(str, '\"');
			if (!str)
				return (quotes);
			quotes++;
			str++;
		}
		else if (*str == '\'')
		{
			str++;
			quotes++;
			str = ft_strchr(str, '\'');
			if (!str)
				return (quotes);
			quotes++;
			str++;
		}
		if (*str == '\0')
			continue;
	}
	return (quotes);
}

char *vars(char **str, t_list *head)
{
	char *var;
	char *newvar;
	int j;
	int len;

	newvar = NULL;
	while (head)
	{
		j = 0;
		while ((*str)[j] && head->content[j] && head->content[j] == (*str)[j])
			j++;
		if (head->content[j] == '=' && ((*str)[j] == ' ' || (*str)[j] == '\0' || (*str)[j] == '\'' || (*str)[j] == '\"' || (*str)[j] == '$'))
		{
			j++;
			newvar = ft_substr(&(head->content[j]), 0, ft_strlen(&(head->content[j])));
			return (newvar);
		}
		head = head->next;
	}
	if (newvar == NULL)
	{
		newvar = (char *)malloc(1);
		*newvar = '\0';
	}
	return (newvar);
}

void dollar(char **str, char **newstr, char **start, t_info *info)
{
	char *tmp;
	char *glue;
	char *var;

	(*str)++;
	if (**str == ' ' || **str == '\0')
	{
		tmp = *newstr;
		*newstr = ft_strjoin(tmp, "$");
		free(tmp);
		return;
	}
	if ((info->d_quote == 0 && info->s_quote == 1))
	{
		tmp = *newstr;
		glue = ft_substr(*start, 0, *str - *start);
		*newstr = ft_strjoin(tmp, glue);
		free(tmp);
		free(glue);
		*start = *str;
		while (**str != '\0' && **str != '\'' && **str != '\"' && **str != ' ')
			(*str)++;
		tmp = *newstr;
		glue = ft_substr(*start, 0, *str - *start);
		*newstr = ft_strjoin(tmp, glue);
		free(tmp);
		free(glue);
	}
	else
	{
		var = vars(str, info->head);
		glue = ft_substr(*start, 0, *str - *start - 1);
		tmp = *newstr;
		*newstr = ft_strjoin(tmp, glue);
		free(tmp);
		free(glue);
		tmp = *newstr;
		*newstr = ft_strjoin(tmp, var);
		free(tmp);
		free(var);
		while (**str != ' ' && **str != '\0' && **str != '\'' && **str != '\"' && **str != '$')
			(*str)++;
	}
}

char *replace_vars(char *str, t_info *info)
{
	int i;
	char *newstr;
	char *start;
	char *glue;
	char *tmp;

	info->d_quote = 0;
	info->s_quote = 0;
	start = str;
	newstr = (char *)malloc(1);
	*newstr = '\0';
	while (*str)
	{
		while (*str && *str != '\'' && *str != '\"' && *str != '$')
			str++;
		if (*str == '\"')
		{
			if (info->s_quote == 0)
				info->d_quote = !(info->d_quote);
			str++;
		}
		else if (*str == '\'')
		{
			if (info->d_quote == 0)
				info->s_quote = !(info->s_quote);
			str++;
		}
		else if (*str == '$')
		{
			dollar(&str, &newstr, &start, info);
			start = str;
		}
		if (*str == '\0')
		{
			glue = ft_substr(start, 0, str - start);
			tmp = newstr;
			newstr = ft_strjoin(tmp, glue);
			free(tmp);
			free(glue);
		}
	}
	return newstr;
}

char **make_tokens(char *str)
{
	char **arr;
	char *start;
	char **new_arr;
	char **tmp;
	int i;
	int j;

	start = str;
	i = 0;

	arr = (char **)malloc(sizeof(char *));
	arr[0] = NULL;
	while (*str)
	{
		while (*str && *str == ' ')
			str++;
		if (*str == '\0')
			continue;
		start = str;
		if (*str != '\"' && *str != '\'')
		{
			if (*str == '|' || *str == '<' || *str == '>')
			{
				i++;
				tmp = arr;
				arr = (char **)malloc(sizeof(char *) * (i + 1));
				arr[i] = NULL;

				j = 0;
				while (tmp[j])
				{
					arr[j] = ft_substr(tmp[j], 0, ft_strlen(tmp[j]));
					j++;
				}
				if (*(str + 1) == '>' || *(str + 1) == '<')
				{
					arr[j] = ft_substr(start, 0, 2);
					str += 2;
				}

				else
				{
					arr[j] = ft_substr(start, 0, 1);
					str++;
				}
				j = 0;
				while (j < i)
				{
					free(tmp[j]);
					j++;
				}
				free(tmp);
				continue;
			}
			while (*str && *str != ' ')
			{
				while (*str && *str != '\"' && *str != '\'' && *str != ' ' && *str != '|' && *str != '>' && *str != '<')
					str++;
				if (*str == '\'')
				{
					str++;
					str = ft_strchr(str, '\'');
					str++;
				}
				else if (*str == '\"')
				{
					str++;
					str = ft_strchr(str, '\"');
					str++;
				}
				else if (*str == '|' || *str == '>' || *str == '<')
					break;
			}
		}
		else if (*str == '\"' || *str == '\'')
		{
			if (*str == '\'')
			{
				str++;
				str = ft_strchr(str, '\'');
				while (*str && *str != ' ' && *str != '|' && *str != '<' && *str != '>')
					str++;
			}

			else if (*str == '\"')
			{
				str++;
				str = ft_strchr(str, '\"');
				while (*str && *str != ' ' && *str != '|' && *str != '<' && *str != '>')
					str++;
			}
		}
		i++;
		tmp = arr;
		arr = (char **)malloc(sizeof(char *) * (i + 1));
		arr[i] = NULL;

		j = 0;
		while (tmp[j])
		{
			arr[j] = ft_substr(tmp[j], 0, ft_strlen(tmp[j]));
			j++;
		}
		arr[j] = ft_substr(start, 0, str - start);
		j = 0;
		while (j < i)
		{
			free(tmp[j]);
			j++;
		}
		free(tmp);
	}

	return (arr);
}

void handle_token(char *tmp_token, t_token *token)
{
	int i;
	int j;
	int quotes;
	int flag;

	i = 0;
	quotes = 0;
	while (tmp_token[i])
	{
		if (tmp_token[i] == '\'' || tmp_token[i] == '\"')
			quotes++;
		i++;
	}
	token->str = (char *)malloc(ft_strlen(tmp_token) - quotes + 1);
	i = 0;
	j = 0;
	flag = 0;
	while (tmp_token[i])
	{
		if (tmp_token[i] == '\'' && flag == 0)
		{
			flag = 1;
			i++;
		}
		else if (tmp_token[i] == '\'' && flag == 1)
		{
			flag = 0;
			i++;
		}
		else if (tmp_token[i] == '\"' && flag == 0)
		{
			flag = 2;
			i++;
		}
		else if (tmp_token[i] == '\"' && flag == 2)
		{
			flag = 0;
			i++;
		}
		if (flag == 1)
		{
			while (tmp_token[i] && tmp_token[i] != '\'')
			{
				token->str[j] = tmp_token[i];
				i++;
				j++;
			}
		}
		else if (flag == 2)
		{
			while (tmp_token[i] && tmp_token[i] != '\"')
			{
				token->str[j] = tmp_token[i];
				i++;
				j++;
			}
		}
		else
		{
			while (tmp_token[i] && tmp_token[i] != '\"' && tmp_token[i] != '\'')
			{
				token->str[j] = tmp_token[i];
				i++;
				j++;
			}
		}
		if (tmp_token[i])
			i++;
	}
	token->str[j] = '\0';
}

t_token *delete_quotes(char **tmp_arr)
{
	int i;
	t_token *tokens_arr;

	i = 0;
	while (tmp_arr[i])
		i++;
	tokens_arr = (t_token *)malloc(sizeof(t_token) * (i + 1));
	tokens_arr[i].str = NULL;
	i = 0;
	while (tmp_arr[i])
	{
		handle_token(tmp_arr[i], &tokens_arr[i]);
		if (tmp_arr[i][0] == '|' && tmp_arr[i][1] == '\0')
			tokens_arr[i].type = "pipe";
		else if (tmp_arr[i][0] == '>' && tmp_arr[i][1] == '\0')
			tokens_arr[i].type = "great";
		else if (tmp_arr[i][0] == '<' && tmp_arr[i][1] == '\0')
			tokens_arr[i].type = "less";
		else if (tmp_arr[i][0] == '>' && tmp_arr[i][1] == '>' && tmp_arr[i][2] == '\0')
			tokens_arr[i].type = "GREAT";
		else if (tmp_arr[i][0] == '<' && tmp_arr[i][1] == '<' && tmp_arr[i][2] == '\0')
			tokens_arr[i].type = "LESS";
		else
			tokens_arr[i].type = "word";
		i++;
	}
	return (tokens_arr);
}

void less_args(t_token *tokens, int i)
{
	int q;
	int j;

	if ((i != 0) && (tokens[i - 1].type[0] != 'p'))
	{
		q = 0;
		i++;
		while (tokens[i].str && tokens[i].type[0] == 'w')
		{
			i++;
			q++;
		}
		i -= q + 1;
		tokens[i].args = (char **)malloc(sizeof(char *) * (q + 1));
		tokens[i].args[q] = NULL;
		j = 0;
		while (q)
		{
			tokens[i].args[j] = ft_strdup(tokens[i + j + 1].str);
			j++;
			q--;
		}
	}
	else
	{
		tokens[i].args = (char **)malloc(sizeof(char *) * 2);
		tokens[i].args[1] = NULL;
		tokens[i].args[0] = ft_strdup(tokens[i + 1].str);
		if (tokens[i + 2].str)
			if (tokens[i + 2].type[0] == 'w')
				tokens[i + 2].type = "command";
	}
}

void set_args(t_info *info)
{
	int i;

	i = 0;
	while (info->tokens[i].str)
	{
		info->tokens[i].args = (char **)malloc(sizeof(char *));
		info->tokens[i].args[0] = NULL;
		i++;
	}
}

void command_args(t_token *tokens, int i)
{
	int q;
	int j;
	int k;

	q = 0;
	j = i;
	i++;
	while (tokens[i].str && tokens[i].type[0] == 'w')
	{
		i++;
		q++;
	}
	if (tokens[i].str)
	{
		if (tokens[i].type[0] == 'g' || tokens[i].type[0] == 'G')
		{
			i += 2;
			while (tokens[i].str && tokens[i].type[0] == 'w')
			{
				i++;
				q++;
			}
		}
	}
	tokens[j].args = (char **)malloc(sizeof(char *) * (q + 2)); //
	tokens[j].args[q + 1] = NULL;
	tokens[j].args[0] = ft_strdup(tokens[j].str);
	k = 0;
	while (tokens[j + k + 1].str && tokens[j + k + 1].type[0] == 'w')
	{
		tokens[j].args[k + 1] = ft_strdup(tokens[j + k + 1].str);
		k++;
	}
	i = j + k + 1;
	if (tokens[i].str)
	{
		if (tokens[i].type[0] == 'g' || tokens[i].type[0] == 'G')
		{
			i += 2;
			while (tokens[i].str && tokens[i].type[0] == 'w')
			{
				tokens[j].args[k + 1] = ft_strdup(tokens[i].str);
				k++;
				i++;
			}
		}
	}
}

void define_types(t_info *info)
{
	int i;

	i = 0;
	while (info->tokens[i].str)
	{
		info->tokens[i].b_in = 0;
		if ((i == 0 && info->tokens[i].type[0] == 'w') || info->tokens[i].type[0] == 'c')
		{
			info->tokens[i].type = "command";
			free(info->tokens[i].args[0]);
			free(info->tokens[i].args);
			command_args(info->tokens, i);
		}
		else if (info->tokens[i].type[0] == 'p')
		{
			if (info->tokens[i + 1].str)
				if (info->tokens[i + 1].type[0] == 'w')
					info->tokens[i + 1].type = "command";
		}
		else if (info->tokens[i].type[0] == 'g' || info->tokens[i].type[0] == 'G')
		{
			free(info->tokens[i].args[0]);
			free(info->tokens[i].args);
			info->tokens[i].args = (char **)malloc(sizeof(char *) * 2);
			info->tokens[i].args[0] = ft_strdup(info->tokens[i + 1].str);
			info->tokens[i].args[1] = NULL;
			if (!((i != 0) && (info->tokens[i - 1].type[0] != 'p')))
				if (info->tokens[i + 2].str)
					if (info->tokens[i + 2].type[0] == 'w')
						info->tokens[i + 2].type = "command";
		}
		else if (info->tokens[i].type[0] == 'l')
		{
			free(info->tokens[i].args[0]);
			free(info->tokens[i].args);
			less_args(info->tokens, i);
		}
		i++;
	}
}

void set_pipes(t_info *info)
{
	int i;
	int j;

	i = -1;
	info->pipes_num = 0;
	while (info->tokens[++i].str)
		info->tokens[i].pipe = 0;
	i = -1;
	while (info->tokens[++i].str)
	{
		if (info->tokens[i].type[0] == 'c')
		{
			j = i + 1;
			while (info->tokens[j].str)
			{
				if (info->tokens[j].type[0] == 'p')
					info->tokens[i].pipe = 1;
				j++;
			}
		}
	}
	i = -1;
	while (info->tokens[++i].str)
		if (info->tokens[i].type[0] == 'p')
			info->pipes_num++;
}

void command_types(t_info *info)
{
	int i;

	i = 0;
	while (info->tokens[i].str)
	{
		if ( (ft_strlen(info->tokens[i].str) == 3 && !ft_strncmp(info->tokens[i].str, "pwd", 3) && info->tokens[i].type[0] == 'c')
		|| (!ft_strncmp(info->tokens[i].str, "cd", 2) && ft_strlen(info->tokens[i].str) == 2 && info->tokens[i].type[0] == 'c')
		|| (ft_strlen(info->tokens[i].str) == 4 && !ft_strncmp(info->tokens[i].str, "echo", 4) && info->tokens[i].type[0] == 'c')
		|| (ft_strlen(info->tokens[i].str) == 3 && !ft_strncmp(info->tokens[i].str, "env", 3) && info->tokens[i].type[0] == 'c')
		|| (ft_strlen(info->tokens[i].str) == 6 && !ft_strncmp(info->tokens[i].str, "export", 6) && info->tokens[i].type[0] == 'c')
		|| (ft_strlen(info->tokens[i].str) == 5 && !ft_strncmp(info->tokens[i].str, "unset", 5) && info->tokens[i].type[0] == 'c')
		|| (ft_strlen(info->tokens[i].str) == 4 && !ft_strncmp(info->tokens[i].str, "exit", 4) && info->tokens[i].type[0] == 'c')
		)
			info->tokens[i].b_in = 1;
		i++;
	}
}