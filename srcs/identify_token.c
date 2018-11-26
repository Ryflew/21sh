/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   identify_token.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/10 20:27:38 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/10/10 20:27:39 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

static void		is_here_op(t_lexer *lexer, t_token **token)
{
	if (*lexer->line == '`')
	{
		if (!lexer->bqt)
			*token = new_token(lexer, BQT, "`", lexer->blank);
		else
			*token = new_token(lexer, EBQT, "`", lexer->blank);
		lexer->bqt = !lexer->bqt;
	}
	else if (*lexer->line == '$' && ft_isalnum(*(lexer->line + 1)))
		*token = new_token(lexer, VAR_OP, "$", lexer->blank);
}

static void		is_other_op(t_lexer *lexer, t_token **token, \
					t_token *last_token)
{
	if (*lexer->line == ',' && lexer->brc && !lexer->bkt)
		*token = new_token(lexer, COM, ",", lexer->blank);
	else if (*lexer->line == '*')
		*token = new_token(lexer, S_WILDCARD, "*", lexer->blank);
	else if (*lexer->line == '?')
		*token = new_token(lexer, Q_WILDCARD, "?", lexer->blank);
	else if (*lexer->line == '(')
	{
		*token = new_token(lexer, LPAR, "(", lexer->blank);
		lexer->par = 1;
	}
	else if (*lexer->line == ')')
	{
		*token = new_token(lexer, RPAR, ")", lexer->blank);
		lexer->par = 0;
	}
	else if (*lexer->line == '!')
		*token = new_token(lexer, HIST, "!", lexer->blank);
	else if (*lexer->line == ';')
		*token = new_token(lexer, SCL, ";", lexer->blank);
	else if (*lexer->line == '-' && last_token
			&& (last_token->type == FRED || last_token->type == BRED))
		*token = new_token(lexer, CLOSE_FD, "-", lexer->blank);
	else if (*lexer->line == '=' && !lexer->blank && last_token &&
			(last_token->type == ASCII_WORD || last_token->type == NUM))
	{
		*token = new_token(lexer, EQUAL, "=", lexer->blank);
		last_token->type = VAR_WORD;
	}
}

static void		is_limit_glob_op(t_lexer *lexer, t_token **token,
								t_token *l_tk)
{
	if (*lexer->line == '[')
	{
		if (*(lexer->line + 1) && (*(lexer->line + 1) == '!'
			|| *(lexer->line + 1) == '^'))
			*token = new_token(lexer, E_WILDCARD, "[!", lexer->blank);
		else
			*token = new_token(lexer, LBKT, "[", lexer->blank);
		lexer->bkt = 1;
	}
	else if (*lexer->line == ']' && lexer->bkt && l_tk && (l_tk->type != LBKT && l_tk->type != E_WILDCARD))
	{
		*token = new_token(lexer, RBKT, "]", lexer->blank);
		lexer->bkt = 0;
	}
	else if (*lexer->line == '{' && !lexer->bkt)
	{
		*token = new_token(lexer, LBRC, "{", lexer->blank);
		++(lexer->brc);
	}
	else if (*lexer->line == '}' && lexer->brc && !lexer->bkt)
	{
		*token = new_token(lexer, RBRC, "}", lexer->blank);
		--(lexer->brc);
	}
	else if (l_tk && l_tk->type == START_RANGE_EXPR)
		*token = new_token(lexer, DASH, "-", lexer->blank);
}

static t_token	*is_reg_op(t_lexer *lexer)
{
	t_token *token;

	token = NULL;
	if (*lexer->line == '>' && *(lexer->line + 1) == '>')
		token = new_token(lexer, DCHEVF, ">>", lexer->blank);
	else if (*lexer->line == '<' && *(lexer->line + 1) == '<')
		token = new_token(lexer, DCHEVB, "<<", lexer->blank);
	else if (*lexer->line == '>' && *(lexer->line + 1) == '&')
		token = new_token(lexer, FRED, ">&", lexer->blank);
	else if (*lexer->line == '<' && *(lexer->line + 1) == '&')
		token = new_token(lexer, BRED, "<&", lexer->blank);
	else if (*lexer->line == '>')
		token = new_token(lexer, CHEVF, ">", lexer->blank);
	else if (*lexer->line == '<')
		token = new_token(lexer, CHEVB, "<", lexer->blank);
	else if (*lexer->line == '&' && *(lexer->line + 1) == '&')
		token = new_token(lexer, AND, "&&", lexer->blank);
	else if (*lexer->line == '|' && *(lexer->line + 1) == '|')
		token = new_token(lexer, OR, "||", lexer->blank);
	else if (*lexer->line == '|')
		token = new_token(lexer, PIPE, "|", lexer->blank);
	return (token);
}

t_token			*identify_token(t_lexer *lexer, t_token *l_tk)
{
	t_token *token;

	token = NULL;
	if (!token && isnt_here_and_bqt(lexer))
		token = is_reg_op(lexer);
	if (!token && isnt_here_and_bqt(lexer))
		is_limit_glob_op(lexer, &token, l_tk);
	if (!token && isnt_here_and_bqt(lexer))
		is_other_op(lexer, &token, l_tk);
	if (!token)
		is_here_op(lexer, &token);
	if (*lexer->line && !(token))
		token = lex_word(lexer, l_tk);
	if (isnt_here_and_bqt(lexer) && token && is_glob_token(TYPE) &&
	!lexer->blank && l_tk && (l_tk->type == WORD ||
		l_tk->type == TILD || l_tk->type == NUM || l_tk->type == ASCII_WORD))
	{
		if (l_tk->type == TILD)
			l_tk->type = TILD_EXPR;
		else
			l_tk->type = EXPR;
	}
	return (token);
}
