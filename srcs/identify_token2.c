/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   identify_token2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/09 17:39:52 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/12/09 17:42:06 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

static void		is_other_op2(t_lexer *lx, t_token **token, t_token *l_tk)
{
	if (*lx->line == '=' && !lx->blank && l_tk && \
		(l_tk->type == ASCII_WORD || l_tk->type == NUM || \
		l_tk->type == NEG_NUM))
	{
		*token = new_token(lx, EQUAL, "=", lx->blank);
		l_tk->type = VAR_WORD;
	}
	else if (*lx->line == '(')
	{
		ft_node_push_front(&lx->st_ops, &(*lx->line));
		*token = new_token(lx, LPAR, "(", lx->blank);
		lx->par = 1;
	}
	else if (*lx->line == ')')
	{
		ft_pop_node(&lx->st_ops, NULL);
		*token = new_token(lx, RPAR, ")", lx->blank);
		lx->par = 0;
	}
}

void			is_other_op(t_lexer *lx, t_token **token, t_token *l_tk)
{
	if (*lx->line == ',' && lx->brc)
		*token = new_token(lx, COM, ",", lx->blank);
	else if (*lx->line == '*')
		*token = new_token(lx, S_WILDCARD, "*", lx->blank);
	else if (*lx->line == '?')
		*token = new_token(lx, Q_WILDCARD, "?", lx->blank);
	else if (*lx->line == '!')
		*token = new_token(lx, HIST, "!", lx->blank);
	else if (*lx->line == ';')
		*token = new_token(lx, SCL, ";", lx->blank);
	else if (*lx->line == '-' && l_tk && (l_tk->type == FRED || \
		l_tk->type == BRED))
		*token = new_token(lx, CLOSE_FD, "-", lx->blank);
	else
		is_other_op2(lx, token, l_tk);
}
