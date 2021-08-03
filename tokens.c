#include "libft/libft.h"

typedef struct s_info
{
	int		d_quote;
	int		s_quote;
	char	first;
	t_list	*head;
} t_info;

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
		return ;
    }
	if ((info->d_quote == 0 && info->s_quote == 1) || (info->s_quote == 1 && info->first == 's'))
	{
		tmp = *newstr;
		glue = ft_substr(*start, 0, *str - *start);
		*newstr = ft_strjoin(tmp, glue);
        free(tmp);
        free(glue);
		*start = *str;
		while (**str != '\0' && **str !='\'' && **str !='\"' && **str !=' ')
			(*str)++;
        tmp = *newstr;
		glue = ft_substr(*start, 0, *str - *start);
        *newstr = ft_strjoin(tmp, glue);
        free(tmp);
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
	newstr = (char*)malloc(1);
	*newstr = '\0';
	while (*str)
	{
		while (*str && *str != '\'' && *str !='\"' && *str != '$')
			str++;
		if (*str == '\"')
		{
			info->d_quote = !(info->d_quote);
			if (info->first == 'n')
				info->first = 'd';
			str++;
		}
		else if (*str == '\'')
		{
			info->s_quote = !(info->s_quote);
			if (info->first == 'n')
				info->first = 's';
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
		}
	}
	return newstr;
}

int main(int ac, char **av, char **envp)
{
	t_info  info;
	make_env(envp, &info.head);
	char *newstr;

	char *str = "\" \'$PWD\'\"  '$PWD'";
	newstr = replace_vars(str, &info);
	printf("%s\n", newstr);
	return 0;
}
