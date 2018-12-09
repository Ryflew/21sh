/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/10 20:26:01 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/12/09 17:02:32 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

static void	replace_expr_by_word(t_list **lexems, t_list *tmp)
{
	t_token *token;

	while (tmp)
	{
		token = (t_token*)tmp->data;
		if (TYPE == END_EXPR)
		{
			if (*lexems == tmp)
				*lexems = tmp->next;
			ft_pop_node(&tmp, (void*)&clear_lexems);
		}
		else
		{
			if (TYPE == EXPR)
				TYPE = WORD;
			if (TYPE == SON)
			{
				if (*lexems == tmp)
					*lexems = tmp->next;
				ft_pop_node(&tmp, (void*)&clear_lexems);
			}
			else
				tmp = tmp->next;
		}
	}
}

void		glob(t_list **first_lexems)
{
	t_list	*lexems;
	t_token	*token;

	lexems = *first_lexems;
	while (lexems)
	{
		token = (t_token*)lexems->data;
		if (TYPE == LBRC)
		{
			if (lexems->next && \
				((t_token*)lexems->next->data)->type == NUM_EXPR)
				manage_range_brc(lexems->next);
			else
				manage_brc(lexems->next);
			clear_old_expr(&lexems, first_lexems, 1);
			merge_expr(*first_lexems);
		}
		else
			lexems = lexems->next;
	}
	replace_all_exprs(first_lexems);
	replace_expr_by_word(first_lexems, *first_lexems);
}
