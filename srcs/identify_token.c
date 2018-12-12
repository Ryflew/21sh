/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   identify_token.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdurst2812 <bdurst2812@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/10 20:27:38 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/12/12 02:30:11 by bdurst2812       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

static void		is_here_op(t_lexer *lx, t_token **token)
{
	if (*lx->line == '`')
	{
		if (!lx->bqt)
		{
			ft_node_push_front(&lx->st_ops, &(*lx->line));
			*token = new_token(lx, BQT, "`", lx->blank);
		}
		else
		{
			ft_pop_node(&lx->st_ops, NULL);
			*token = (lx->st_ops && *((char*)lx->st_ops->data) == '"') ? \
			new_token(lx, EBQT_INSIDE_ST_OP, "`", lx->blank) : \
			new_token(lx, EBQT, "`", lx->blank);
		}
		lx->bqt = !lx->bqt;
	}
	else if (*lx->line == '$' && ft_isalnum(*(lx->line + 1)))
		*token = new_token(lx, VAR_OP, "$", lx->blank);
	else if (lx->range_brc && *lx->line == '.')
		*token = new_token(lx, NUM_RANGE_EXPR, "..", lx->blank);
}

static void		is_limit_glob_op2(t_lexer *lx, t_token **token, t_token *l_tk)
{
	if (*lx->line == '}' && lx->brc && !lx->bkt)
	{
		*token = new_token(lx, RBRC, "}", lx->blank);
		--(lx->brc);
		lx->range_brc = 0;
	}
	else if (l_tk && l_tk->type == START_RANGE_EXPR)
		*token = new_token(lx, DASH, "-", lx->blank);
}

static void		is_limit_glob_op(t_lexer *lx, t_token **token, t_token *l_tk)
{
	if (*lx->line == '[' && is_lbkt(lx, 0))
	{
		ft_node_push_front(&lx->st_ops, &(*lx->line));
		*token = (*(lx->line + 1) && (*(lx->line + 1) == '!' \
			|| *(lx->line + 1) == '^')) ? new_token(lx, E_WILDCARD, "[!",\
		lx->blank) : new_token(lx, LBKT, "[", lx->blank);
		lx->bkt = 1;
	}
	else if (*lx->line == ']' && lx->bkt && l_tk && l_tk->type != LBKT && \
		l_tk->type != E_WILDCARD)
	{
		ft_pop_node(&lx->st_ops, NULL);
		*token = new_token(lx, RBKT, "]", lx->blank);
		lx->bkt = 0;
	}
	else if (*lx->line == '{' && (is_lbrc(lx, 0) || is_num_range_expr(lx, 0)))
	{
		*token = new_token(lx, LBRC, "{", lx->blank);
		++(lx->brc);
	}
	else
		is_limit_glob_op2(lx, token, l_tk);
}

static t_token	*is_reg_op(t_lexer *lx)
{
	t_token *token;

	token = NULL;
	if (*lx->line == '>' && *(lx->line + 1) == '>')
		token = new_token(lx, DCHEVF, ">>", lx->blank);
	else if (*lx->line == '<' && *(lx->line + 1) == '<')
		token = new_token(lx, DCHEVB, "<<", lx->blank);
	else if (*lx->line == '>' && *(lx->line + 1) == '&')
		token = new_token(lx, FRED, ">&", lx->blank);
	else if (*lx->line == '<' && *(lx->line + 1) == '&')
		token = new_token(lx, BRED, "<&", lx->blank);
	else if (*lx->line == '>')
		token = new_token(lx, CHEVF, ">", lx->blank);
	else if (*lx->line == '<')
		token = new_token(lx, CHEVB, "<", lx->blank);
	else if (*lx->line == '&' && *(lx->line + 1) == '&')
		token = new_token(lx, AND, "&&", lx->blank);
	else if (*lx->line == '|' && *(lx->line + 1) == '|')
		token = new_token(lx, OR, "||", lx->blank);
	else if (*lx->line == '|')
		token = new_token(lx, PIPE, "|", lx->blank);
	return (token);
}

t_token			*identify_token(t_lexer *lx, t_token *l_tk)
{
	t_token *token;

	token = NULL;
	if (!token && isnt_here_and_bqt(lx) && !lx->bkt)
		token = is_reg_op(lx);
	if (!token && isnt_here_and_bqt(lx))
		is_limit_glob_op(lx, &token, l_tk);
	if (!token && isnt_here_and_bqt(lx) && !lx->bkt)
		is_other_op(lx, &token, l_tk);
	if (!token && !lx->bkt)
		is_here_op(lx, &token);
	if (*lx->line && !(token))
		token = lex_word(lx, l_tk);
	if (isnt_here_and_bqt(lx) && token && is_glob_token(TYPE) &&
	!lx->blank && l_tk && (l_tk->type == WORD ||
		l_tk->type == TILD || l_tk->type == NUM || l_tk->type == ASCII_WORD || \
		l_tk->type == NEG_NUM))
	{
		if (l_tk->type == TILD)
			l_tk->type = TILD_EXPR;
		else
			l_tk->type = EXPR;
	}
	return (token);
}
