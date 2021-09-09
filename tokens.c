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
		if (ft_strncmp("_=./", envp[i], 4) && ft_strncmp("OLDPWD=", envp[i], 7))
		{
	        if (!ft_strncmp(envp[i], "SHLVL=", 6))
            	new->content = change_shlvl(envp[i]);
			else
				new->content = ft_strdup(envp[i]);
			new->next = NULL;
			ft_lstadd_back(head, new);
		}
		else
			free(new);
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




void less_args(t_token *tokens, int i)
{
	int q;
	int j;

	if ((i != 0) && (tokens[i - 1].type != 'p'))
	{
		q = 0;
		i++;
		while (tokens[i].str && tokens[i].type == 'w')
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
			if (tokens[i + 2].type == 'w')
				tokens[i + 2].type = 'c';
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
	while (tokens[i].str && tokens[i].type == 'w')
	{
		i++;
		q++;
	}
	if (tokens[i].str)
	{
		if (tokens[i].type == 'g' || tokens[i].type == 'G')
		{
			i += 2;
			while (tokens[i].str && tokens[i].type == 'w')
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
	while (tokens[j + k + 1].str && tokens[j + k + 1].type == 'w')
	{
		tokens[j].args[k + 1] = ft_strdup(tokens[j + k + 1].str);
		k++;
	}
	i = j + k + 1;
	if (tokens[i].str)
	{
		if (tokens[i].type == 'g' || tokens[i].type == 'G')
		{
			i += 2;
			while (tokens[i].str && tokens[i].type == 'w')
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
		info->tokens[i].print = 1;
		if ((i == 0 && info->tokens[i].type == 'w') || info->tokens[i].type == 'c')
		{
			info->tokens[i].type = 'c';
			free(info->tokens[i].args[0]);
			free(info->tokens[i].args);
			command_args(info->tokens, i);
		}
		else if (info->tokens[i].type == 'p')
		{
			if (info->tokens[i + 1].str)
				if (info->tokens[i + 1].type == 'w')
					info->tokens[i + 1].type = 'c';
		}
		else if (info->tokens[i].type == 'g' || info->tokens[i].type == 'G')
		{
			free(info->tokens[i].args[0]);
			free(info->tokens[i].args);
			info->tokens[i].args = (char **)malloc(sizeof(char *) * 2);
			info->tokens[i].args[0] = ft_strdup(info->tokens[i + 1].str);
			info->tokens[i].args[1] = NULL;
			if (!((i != 0) && (info->tokens[i - 1].type != 'p')))
				if (info->tokens[i + 2].str)
					if (info->tokens[i + 2].type == 'w')
						info->tokens[i + 2].type = 'c';
		}
		else if (info->tokens[i].type == 'l')
		{
			free(info->tokens[i].args[0]);
			free(info->tokens[i].args);
			less_args(info->tokens, i);
		}
		else if (info->tokens[i].type == 'L')
		{
			free(info->tokens[i].args[0]);
			free(info->tokens[i].args);
			info->tokens[i].args = (char **)malloc(sizeof(char *) * 2);
			info->tokens[i].args[0] = ft_strdup(info->tokens[i + 1]. str);
			info->tokens[i].args[1] = NULL;
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
		if (info->tokens[i].type == 'c')
		{
			j = i + 1;
			while (info->tokens[j].str)
			{
				if (info->tokens[j].type == 'p')
					info->tokens[i].pipe = 1;
				j++;
			}
		}
	}
	i = -1;
	while (info->tokens[++i].str)
		if (info->tokens[i].type == 'p')
			info->pipes_num++;
}

void command_types(t_info *info)
{
	int i;

	i = 0;
	while (info->tokens[i].str)
	{
		if ( (!ft_strncmp(info->tokens[i].str, "cd", 2) && ft_strlen(info->tokens[i].str) == 2 && info->tokens[i].type == 'c')
		|| (ft_strlen(info->tokens[i].str) == 6 && !ft_strncmp(info->tokens[i].str, "export", 6) && info->tokens[i].type == 'c' && info->tokens[i].args[1])
		|| (ft_strlen(info->tokens[i].str) == 5 && !ft_strncmp(info->tokens[i].str, "unset", 5) && info->tokens[i].type == 'c')
		|| (ft_strlen(info->tokens[i].str) == 4 && !ft_strncmp(info->tokens[i].str, "exit", 4) && info->tokens[i].type == 'c')
		)
			info->tokens[i].print = 0;
		i++;
	}
}
