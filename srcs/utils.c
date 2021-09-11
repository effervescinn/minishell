#include "../minishell.h"

int	ft_putchar(int c)
{
	write(1, &c, 1);
	return (1);
}

char	*no_leaks_join(char *str1, char *str2)
{
	char	*tmp;

	tmp = str1;
	str1 = ft_strjoin(str1, str2);
	free(tmp);
	return (str1);
}

void	start_of_line(t_info *info)
{
	info->i2 = info->i;
	while (info->i2 != 0 && info->tokens[info->i2].type != 'p')
		info->i2--;
	if (info->tokens[info->i2].type == 'p')
		info->i2++;
}

char	*change_shlvl(char *str)
{
	int		lvl;
	char	*tmp;
	char	*new_str;

	lvl = ft_atoi(str + 6);
	lvl++;
	tmp = ft_itoa(lvl);
	new_str = ft_strjoin("SHLVL=", tmp);
	free(tmp);
	return (new_str);
}

char	**make_envp_arr(t_info *info)
{
	int		i;
	t_list	*tmp;
	char	**res;

	i = 0;
	tmp = info->head;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	res = (char **)malloc(sizeof(char *) * (i + 1));
	res[i] = NULL;
	tmp = info->head;
	i = 0;
	while (tmp)
	{
		res[i] = tmp->content;
		tmp = tmp->next;
		i++;
	}
	return (res);
}
