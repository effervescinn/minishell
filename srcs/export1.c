#include "../minishell.h"

void	copy_pwds(t_info *info)
{
	if (info->pwd)
		info->str_pwd = ft_strdup(info->pwd->content);
	if (info->oldpwd)
		info->str_oldpwd = ft_strdup(info->oldpwd->content);
}

void	set_pointers(t_info *info)
{
	t_list	*tmp;

	info->pwd = NULL;
	info->oldpwd = NULL;
	info->pths = NULL;
	tmp = info->head;
	while (tmp)
	{
		if (!ft_strncmp(tmp->content, "OLDPWD=", 7)
			|| !ft_strncmp(tmp->content, "OLDPWD\0", 7))
			info->oldpwd = tmp;
		if (!ft_strncmp(tmp->content, "PWD=", 4)
			|| !ft_strncmp(tmp->content, "PWD\0", 4))
			info->pwd = tmp;
		else if (!ft_strncmp(tmp->content, "PATH=", 5))
			info->pths = tmp;
		tmp = tmp->next;
	}
}

void	swap_content(t_list *list1, t_list *list2)
{
	char	*tmp;

	tmp = list1->content;
	list1->content = list2->content;
	list2->content = tmp;
}

void	export_order(t_info *info, int i)
{
	t_list	*next;
	int		dif;
	t_list	*head;

	head = info->exp;
	while (i--)
	{
		while (info->exp->next)
		{
			next = info->exp->next;
			dif = ft_strncmp(info->exp->content, next->content,
					ft_strlen(info->exp->content));
			if (dif > 0)
				swap_content(info->exp, next);
			info->exp = info->exp->next;
		}
		info->exp = head;
	}
}

void	join_env_and_extra(t_info *info, t_list **exp_tmp, int *i)
{
	t_list	*extra_tmp;

	if (!info->extra_exp)
		(*exp_tmp)->next = NULL;
	else
	{
		extra_tmp = info->extra_exp;
		while (extra_tmp)
		{
			(*exp_tmp)->next = malloc(sizeof(t_list));
			*exp_tmp = (*exp_tmp)->next;
			(*exp_tmp)->content = ft_strdup(extra_tmp->content);
			extra_tmp = extra_tmp->next;
			i++;
		}
		(*exp_tmp)->next = NULL;
	}
}
