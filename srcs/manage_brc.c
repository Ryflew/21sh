/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_brc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/10 20:28:06 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/12/09 18:17:09 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

static void	get_rest_expr(t_list **lexems, t_list **new_lexems)
{
	t_token	*token;

	*lexems = (*lexems)->next;
	while (*lexems)
	{
		token = (t_token*)(*lexems)->data;
		if (TYPE == END_EXPR)
			break ;
		ft_node_push_back(new_lexems, new_token(NULL, TYPE, VAL, BLK));
		*lexems = (*lexems)->next;
	}
}

static void	get_expr_start(t_list *lexems, t_list **new_lexems, \
			enum e_token type)
{
	t_token	*token;
	char	brc;

	brc = 0;
	while (lexems->prev)
	{
		token = (t_token*)lexems->prev->data;
		lexems = lexems->prev;
		if (TYPE == LBRC)
			++brc;
		else if (TYPE == RBRC)
			--brc;
		if (TYPE == type && type == LBRC && brc == 1)
			break ;
	}
	while (lexems->prev)
	{
		token = (t_token*)lexems->prev->data;
		if (is_glob_token(TYPE))
			ft_node_push_front(new_lexems, new_token(NULL, TYPE, VAL, BLK));
		else
			break ;
		lexems = lexems->prev;
	}
}

void		create_expr_from_brc(t_list *lexems, t_token *token,
				t_list **nw_lexems, t_list **nws_lexems)
{
	t_list	*tmp;
	char	brc;

	brc = 0;
	get_expr_start(lexems, nw_lexems, LBRC);
	tmp = lexems;
	while (tmp && (TYPE != RBRC || (brc != -1 && tmp != lexems)))
	{
		tmp = tmp->next;
		token = (t_token*)tmp->data;
		if (TYPE == LBRC)
			++brc;
		else if (TYPE == RBRC)
			--brc;
	}
	if (tmp->next)
		get_rest_expr(&tmp, nw_lexems);
	ft_node_push_back(nw_lexems, new_token(NULL, END_EXPR, "", 0));
	token = (t_token*)lexems->data;
	if (*nws_lexems)
		ft_add_list(ft_go_end_list(*nws_lexems), *nw_lexems);
	else
		*nws_lexems = *nw_lexems;
	*nw_lexems = NULL;
}

void		add_news_expr(t_list *lexems, t_list *begin_news_expr)
{
	t_list	*end_lexems;
	t_token	*token;

	end_lexems = lexems;
	while (end_lexems)
	{
		token = (t_token*)end_lexems->data;
		if (TYPE == END_EXPR)
			break ;
		end_lexems = end_lexems->next;
	}
	ft_add_list(end_lexems, begin_news_expr);
}
