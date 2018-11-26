/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_text.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdurst <bdurst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/11 17:45:59 by bdurst            #+#    #+#             */
/*   Updated: 2017/10/11 17:46:00 by bdurst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

char		eat(t_sh *sh, enum e_token token)
{
	if (sh->current_token->type == token)
	{
		sh->lexer->lexems = sh->lexer->lexems->next;
		if (sh->lexer->lexems)
			sh->current_token = sh->lexer->lexems->data;
		else
			sh->current_token = NULL;
	}
	else
		return (-1);
	return (0);
}

static void	text_rules3(t_sh *sh, t_token **token)
{
	if (sh->current_token->type == VAR_OP)
		eat(sh, VAR_OP);
	else if (sh->current_token->type == VAR_WORD)
		eat(sh, VAR_WORD);
	else if (sh->current_token->type == TILD_VAR_WORD)
		eat(sh, TILD_VAR_WORD);
	else if (sh->current_token->type == EQUAL)
		eat(sh, EQUAL);
	else if (sh->current_token->type == LPAR)
		eat(sh, LPAR);
	else
		*token = NULL;
}

static void	text_rules2(t_sh *sh, t_token **token)
{
	if (sh->current_token->type == S_WILDCARD)
		eat(sh, S_WILDCARD);
	else if (sh->current_token->type == Q_WILDCARD)
		eat(sh, Q_WILDCARD);
	else if (sh->current_token->type == E_WILDCARD)
		eat(sh, E_WILDCARD);
	else if (sh->current_token->type == LBKT)
		eat(sh, LBKT);
	else if (sh->current_token->type == RBKT)
		eat(sh, RBKT);
	else if (sh->current_token->type == LBRC)
		eat(sh, LBRC);
	else if (sh->current_token->type == RBRC)
		eat(sh, RBRC);
	else if (sh->current_token->type == COM)
		eat(sh, COM);
	else if (sh->current_token->type == EXPR)
		eat(sh, EXPR);
	else if (sh->current_token->type == BKT_EXPR)
		eat(sh, BKT_EXPR);
	else if (sh->current_token->type == TILD_EXPR)
		eat(sh, TILD_EXPR);
	else
		text_rules3(sh, token);
}

t_token		*text_rules(t_sh *sh, char is_inside_bqt)
{
	t_token *token;

	if (!sh->current_token)
		return (NULL);
	token = sh->current_token;
	if (is_inside_bqt)
		eat(sh, TYPE);
	else if (sh->current_token->type == WORD)
		eat(sh, WORD);
	else if (sh->current_token->type == NUM)
		eat(sh, NUM);
	else if (sh->current_token->type == TILD)
		eat(sh, TILD);
	else if (sh->current_token->type == END_EXPR)
		eat(sh, END_EXPR);
	else if (sh->current_token->type == START_RANGE_EXPR)
		eat(sh, START_RANGE_EXPR);
	else if (sh->current_token->type == DASH)
		eat(sh, DASH);
	else if (sh->current_token->type == END_RANGE_EXPR)
		eat(sh, END_RANGE_EXPR);
	else
		text_rules2(sh, &token);
	return (token);
}
