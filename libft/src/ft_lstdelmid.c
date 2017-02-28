/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelmid.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/12 00:13:42 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/02/12 00:14:04 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../include/libft.h"

void	ft_lstdelmid(t_list **alst, t_list *previous)
{
	t_list	*tmp;
	t_list	*tmp2;

	tmp = *alst;
	while (tmp && tmp != previous)
		tmp = tmp->next;
	if (!tmp)
		return ;
	tmp2 = tmp->next;
	if (!tmp2)
		return ;
	tmp->next = tmp2->next;
	free(tmp2->content);
	free(tmp2);
}
