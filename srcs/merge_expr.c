/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   merge_expr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdurst <bdurst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/11 17:47:11 by bdurst            #+#    #+#             */
/*   Updated: 2017/10/11 17:47:12 by bdurst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

void	merge_expr(t_list *lexems)
{
	t_token	*token;
	t_token	*next_token;
	char	*to_free;

	while (lexems && lexems->next)
	{
		token = (t_token*)lexems->data;
		next_token = (t_token*)lexems->next->data;
		if (TYPE == EXPR && (!lexems->prev || (next_token->type == END_EXPR && \
			!is_glob_token(((t_token*)lexems->prev->data)->type))))
			TYPE = WORD;
		if ((TYPE == EXPR || TYPE == BKT_EXPR) && (next_token->type == EXPR
			|| next_token->type == BKT_EXPR))
		{
			to_free = VAL;
			VAL = ft_strjoin(VAL, next_token->value);
			free(to_free);
			ft_pop_node(&(lexems->next), (void*)&clear_lexems);
		}
		else
			lexems = lexems->next;
	}
}

void	merge_expr_to_word(t_list *lexems)
{
	t_token	*token;
	t_token	*next_token;
	char	*to_free;

	while (lexems->next)
	{
		token = (t_token*)lexems->data;
		next_token = (t_token*)lexems->next->data;
		if (next_token->type == END_EXPR)
		{
			TYPE = WORD;
			ft_pop_node(&(lexems->next), (void*)&clear_lexems);
			break ;
		}
		to_free = VAL;
		VAL = ft_strjoin(VAL, next_token->value);
		free(to_free);
		ft_pop_node(&(lexems->next), (void*)&clear_lexems);
	}
}
