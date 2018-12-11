/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete_lexems.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/09 20:55:14 by bdurst2812        #+#    #+#             */
/*   Updated: 2018/12/09 22:32:38 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

void		delete_first_subshell_lexems(t_list **first_lexems, t_list **lexems)
{
	if (*first_lexems == *lexems)
		*first_lexems = (*lexems)->next;
	ft_pop_node(lexems, (void*)&clear_lexems);
}

void		delete_subshell_lexems(t_list **first_lexems, t_list **lexems,
								enum e_token open_type, enum e_token close_type)
{
	t_token	*token;
	char	open;

	open = 0;
	while (*lexems)
	{
		token = (t_token*)(*lexems)->data;
		if (TYPE == open_type)
			++open;
		if (TYPE == close_type && TYPE != NONE)
		{
			if (!open)
			{
				delete_first_subshell_lexems(first_lexems, lexems);
				break ;
			}
			--open;
		}
		delete_first_subshell_lexems(first_lexems, lexems);
	}
}
