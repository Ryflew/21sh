/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_brc2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/09 18:15:20 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/12/09 18:17:30 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

static void	manage_range_brc2(t_list *lexems, t_list **new_lexems, \
	t_list **news_lexems, t_token *token)
{
	long	from;
	long	to;

	from = ft_atoi(VAL);
	to = ft_atoi(((t_token*)(lexems->next->next->data))->value);
	if (from <= to)
	{
		while (from <= to)
		{
			ft_node_push_back(new_lexems, new_token(NULL, EXPR, \
				ft_itoa(from++), BLK));
			create_expr_from_brc(lexems, token, new_lexems, \
				news_lexems);
		}
		return ;
	}
	while (from >= to)
	{
		ft_node_push_back(new_lexems, new_token(NULL, EXPR, \
			ft_itoa(from--), BLK));
		create_expr_from_brc(lexems, token, new_lexems, \
			news_lexems);
	}
}

void		manage_range_brc(t_list *lexems)
{
	t_token	*token;
	t_list	*new_lexems;
	t_list	*news_lexems;

	new_lexems = NULL;
	news_lexems = NULL;
	while (lexems)
	{
		token = (t_token*)lexems->data;
		if (TYPE == END_EXPR)
			break ;
		if (TYPE == NUM_EXPR)
		{
			manage_range_brc2(lexems, &new_lexems, &news_lexems, token);
			add_news_expr(lexems, news_lexems);
			return ;
		}
		lexems = lexems->next;
	}
}

static char	manage_brc2(t_token *token, char *brc)
{
	if (TYPE == LBRC)
		++(*brc);
	else if (TYPE == RBRC)
		--(*brc);
	else if (TYPE == END_EXPR)
		return (1);
	return (0);
}

void		manage_brc(t_list *lexems)
{
	t_token	*token;
	t_list	*new_lexems;
	t_list	*news_lexems;
	char	brc;

	new_lexems = NULL;
	news_lexems = NULL;
	brc = 0;
	while (lexems)
	{
		token = (t_token*)lexems->data;
		if (manage_brc2(token, &brc))
			break ;
		if ((TYPE == COM && !brc) || (TYPE == RBRC && brc == -1))
		{
			create_expr_from_brc(lexems, token, &new_lexems, &news_lexems);
			if (TYPE == RBRC)
				return (add_news_expr(lexems, news_lexems));
		}
		else
			ft_node_push_back(&new_lexems, new_token(NULL, TYPE, VAL, BLK));
		lexems = lexems->next;
	}
}
