#include "libft/libft.h"

typedef struct s_token
{
	char *str;
	char *type; //word, command, pipe, great, less, greatgreat, lessless 
} t_token;

typedef struct	s_info
{
	int d_quote;
	int s_quote;
	char first;
	t_list *head;
	t_token *tokens;
}				t_info;

void make_env(char **envp, t_list **head)
{

	t_list *new;
	int i;

	i = 1;
	*head = ft_lstnew(envp[0]);
	while (envp[i])
	{
		new = (t_list *)malloc(sizeof(t_list));
		new->content = envp[i];
		new->next = NULL;
		ft_lstadd_back(head, new);
		i++;
	}
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
	info->first = 'n';
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
				continue ;
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
				str++;
			}

			else if (*str == '\"')
			{
				str++;
				str = ft_strchr(str, '\"');
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

	i = 0;
	quotes = 0;
	while (tmp_token[i])
	{
		if (tmp_token[i] == '\'' || tmp_token[i] == '\"')
			quotes++;
		i++;
	}
	token->str = (char *)malloc(ft_strlen(tmp_token) - quotes + 1);
	i = -1;
	j = 0;
	while (tmp_token[++i])
	{
		if (tmp_token[i] == '\'' || tmp_token[i] == '\"')
			continue;
		token->str[j] = tmp_token[i];
		j++;
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
			tokens_arr[i].type = "greatgreat";
		else if (tmp_arr[i][0] == '<' && tmp_arr[i][1] == '<' && tmp_arr[i][2] == '\0')
			tokens_arr[i].type = "lessless";
		else
			tokens_arr[i].type = "word";
		i++;
	}
	return (tokens_arr);
}

int main(int ac, char **av, char **envp)
{
	t_info info;
	make_env(envp, &info.head);
	char *newstr;
	char **tmp_arr;

	char *str = " \"lalal 'df' topolya\"  \"|\"    '>>' \">>\"ffd|  '$PWD \"$PWD\"' okay>>";
	newstr = replace_vars(str, &info);
	tmp_arr = make_tokens(newstr);
	info.tokens = delete_quotes(tmp_arr);

	int i = 0;
	while (info.tokens[i].str)
	{
		printf("%s || %s\n", info.tokens[i].str, info.tokens[i].type);
		i++;
	}

	// ОТ УТЕЧЕК
	// free(newstr);

	// int i = 0;
	// while (tokens_arr[i])
	// {
	// 	free(tokens_arr[i]);
	// 	i++;
	// }
	// free(tokens_arr);

	// t_list *tmp;
	// while(info.head)
	// {
	// 	tmp = info.head->next;
	// 	free(info.head);
	// 	info.head = tmp;
	// }

	//почистить tmp_arr

	return 0;
}

// gcc -L./libft -lft tokens.c
