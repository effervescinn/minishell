/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnorcros <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/13 19:20:47 by lnorcros          #+#    #+#             */
/*   Updated: 2020/11/13 19:20:48 by lnorcros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*begin_list;
	t_list	*new_lst;

	if (!lst && !f)
		return (NULL);
	if (!(new_lst = ft_lstnew(f(lst->content))))
	{
		ft_lstclear(&new_lst, del);
		return (NULL);
	}
	begin_list = new_lst;
	lst = lst->next;
	while (lst)
	{
		if (!(new_lst = ft_lstnew(f(lst->content))))
		{
			ft_lstclear(&new_lst, del);
			return (begin_list);
		}
		ft_lstadd_back(&begin_list, new_lst);
		lst = lst->next;
	}
	return (begin_list);
}
