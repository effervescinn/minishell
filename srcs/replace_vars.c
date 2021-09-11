#include "../minishell.h"

void	check_syms(char **str, t_info *info, char **newstr, char **start)
{
	while (**str && **str != '\'' && **str != '\"' && **str != '$')
		(*str)++;
	if (**str == '\"')
	{
		if (info->s_quote == 0)
			info->d_quote = !(info->d_quote);
		(*str)++;
	}
	else if (**str == '\'')
	{
		if (info->d_quote == 0)
			info->s_quote = !(info->s_quote);
		(*str)++;
	}
	else if (**str == '$')
	{
		dollar(str, newstr, start, info);
		*start = *str;
	}
}

char	*replace_vars(char *str, t_info *info)
{
	char	*newstr;
	char	*start;
	char	*glue;
	char	*tmp;

	info->d_quote = 0;
	info->s_quote = 0;
	start = str;
	newstr = (char *)malloc(1);
	*newstr = '\0';
	while (*str)
	{
		check_syms(&str, info, &newstr, &start);
		if (*str == '\0')
		{
			glue = ft_substr(start, 0, str - start);
			tmp = newstr;
			newstr = ft_strjoin(tmp, glue);
			free(tmp);
			free(glue);
		}
	}
	return (newstr);
}
