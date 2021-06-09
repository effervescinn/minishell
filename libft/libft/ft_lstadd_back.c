/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnorcros <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/13 19:20:25 by lnorcros          #+#    #+#             */
/*   Updated: 2020/11/13 19:20:27 by lnorcros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*p;

	if (!lst)
		return ;
	if (!*lst)
	{
		(*lst) = new;
		new->next = NULL;
		return ;
	}
	p = *lst;
	while (p->next)
		p = p->next;
	p->next = new;
	new->next = NULL;
}
