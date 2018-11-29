/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_brc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/10 20:28:06 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/11/29 15:00:58 by vdarmaya         ###   ########.fr       */
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
		if (is_glob_token(TYPE) || TYPE == EXPR)
			ft_node_push_front(new_lexems, new_token(NULL, TYPE, VAL, BLK));
		else
			break ;
		lexems = lexems->prev;
	}
}

static char	create_expr_from_brc(t_list *lexems, t_token *token,
				t_list **nw_lexems)
{
	t_list	*tmp;
	char	brc;

	brc = 0;
	if (TYPE != RBRC)
		get_expr_start(lexems, nw_lexems, LBRC);
	tmp = lexems;
	while (tmp && (TYPE != RBRC || brc != -1) && TYPE != END_EXPR)
	{
		tmp = tmp->next;
		token = (t_token*)tmp->data;
		if (TYPE == LBRC)
			++brc;
		else if (TYPE == RBRC)
			--brc;
	}
	if (tmp->next && TYPE != END_EXPR)
		get_rest_expr(&tmp, nw_lexems);
	ft_node_push_back(nw_lexems, new_token(NULL, END_EXPR, "", 0));
	ft_add_list(tmp, *nw_lexems);
	token = (t_token*)lexems->data;
	if (TYPE == RBRC)
		return (0);
	*nw_lexems = NULL;
	return (1);
}

void		manage_brc(t_list *lexems)
{
	t_token	*token;
	t_list	*new_lexems;
	char	brc;

	new_lexems = NULL;
	brc = 0;
	while (lexems)
	{
		token = (t_token*)lexems->data;
		if (TYPE == LBRC)
			++brc;
		else if (TYPE == RBRC)
			--brc;
		else if (TYPE == END_EXPR)
			break ;
		if ((TYPE == COM && !brc) || (TYPE == RBRC && brc == -1))
		{
			if (!(create_expr_from_brc(lexems, token, &new_lexems)))
				break ;
		}
		else
			ft_node_push_back(&new_lexems, new_token(NULL, TYPE, VAL, BLK));
		lexems = lexems->next;
	}
}
