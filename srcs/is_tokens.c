/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_tokens.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/10 20:27:45 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/11/29 14:55:25 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

char	is_string_op(char c)
{
	if (c == '\'' || c == '"')
		return (1);
	return (0);
}

char	is_operator(char c, char c2)
{
	if (c == '>' || c == '<' || (c == '&' && c2 == '&') || c == '|' \
	|| c == ';' || c == '?')
		return (1);
	return (0);
}

char	is_glob_token(enum e_token type)
{
	if (type == S_WILDCARD || type == Q_WILDCARD || type == E_WILDCARD \
		|| type == LBKT || type == RBKT || type == LBRC || type == RBRC \
		|| type == BKT_EXPR || type == START_RANGE_EXPR || type == DASH \
		|| type == END_RANGE_EXPR || type == EXPR || type == NUM_EXPR \
		|| type == NUM_RANGE_EXPR || type == COM)
		return (1);
	return (0);
}

char	is_regular_char_in_here(t_lexer *lexer, char c)
{
	return (lexer->her && c != '`' && c != '$') ? 1 : 0;
}

char	is_start_range_expr(t_lexer *lexer, t_token *last_token, char *line, \
						int i)
{
	if (lexer->bkt && last_token->type != DASH && line[i] && line[i] == '-' \
	&& line[i + 1] && line[i + 1] != ']' && line[i - 1] < line[i + 1])
		return (1);
	return (0);
}
