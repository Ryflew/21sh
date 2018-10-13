/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_string.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdurst <bdurst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/10 20:27:49 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/10/11 19:59:46 by bdurst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

static void	if_its_word(char c, char *bs, char *st_op, enum e_token *type)
{
	if (!ft_isdigit(c))
	{
		if (!ft_isalnum(c))
			*type = WORD;
		else if (*type != WORD)
			*type = ASCII_WORD;
	}
	if (is_string_op(c) && !*st_op)
		*st_op = c;
	else if (c == *st_op)
		*st_op = 0;
	if (c == '\\' && *st_op != '\'' && !*bs)
		*bs = 1;
	else
		*bs = 0;
}

int			compute_word_size(t_lexer *lexer, enum e_token *type, char *st_op,
								t_token *l_tk)
{
	char	bs;
	int		i;

	bs = 0;
	*st_op = 0;
	i = 0;
	while ((lexer->line)[i] && ((l_tk && l_tk->type == VAR_OP &&
	ft_isalnum((lexer->line)[i])) || ((!l_tk || l_tk->type != VAR_OP)
	&& ((bs || (lexer->bqt && (lexer->line)[i] != '`')) || \
	(!is_operator((lexer->line)[i], (lexer->line)[i + 1]) \
	&& !ft_isblank((lexer->line)[i]) && ((lexer->line)[i] != ',' || !lexer->brc
	|| lexer->bkt)) || ((lexer->line)[i] == ']' && !lexer->bkt) ||
	((lexer->line)[i] == '}' && !lexer->brc) || ((lexer->line)[i] == '{' &&
	lexer->bkt) || ((lexer->line)[i] == '}' && lexer->bkt) || (*st_op &&
	(((lexer->line)[i] != '`' && (lexer->line)[i] != '$') || *st_op == '\''))
	|| ((lexer->line)[i] == '$' && !ft_isalnum((lexer->line)[i + 1])) ||
	(((lexer->line)[i] == '=' && !i)))) || (!isnt_here_and_bqt(lexer) &&
	(lexer->line)[i] != '$' && (lexer->line)[i] != '`'
	&& !ft_isblank((lexer->line)[i]))))
		if_its_word((lexer->line)[i++], &bs, st_op, type);
	return (i);
}

static void	find_type(t_lexer *lx, t_token *l_tk, char st_op, \
				enum e_token *type)
{
	if (isnt_here_and_bqt(lx) && *lx->line == '~' && (lx->blank || !l_tk
	|| l_tk->type == LBRC || l_tk->type == COM) && (ft_isblank(*(lx->line + 1))
	|| !*(lx->line + 1) || *(lx->line + 1) == '/' || *(lx->line + 1) == '}' ||
	*(lx->line + 1) == ',') && !st_op)
		*type = TILD;
	else if (l_tk && (l_tk->type == VAR_OP)
			&& !lx->blank)
		*type = VAR_WORD;
	else if (isnt_here_and_bqt(lx) && l_tk && l_tk->type == EQUAL && !lx->blank)
		*type = (*lx->line == '~') ? TILD_VAR_WORD : VAR_WORD;
	if (isnt_here_and_bqt(lx) && l_tk && is_glob_token(l_tk->type) && !lx->blank)
	{
		if (lx->bkt)
			*type = BKT_EXPR;
		else if (*type == TILD)
			*type = TILD_EXPR;
		else
			*type = EXPR;
	}
}

t_token		*lex_word(t_lexer *lexer, t_token *last_token)
{
	int				word_size;
	t_token			*token;
	enum e_token	type;
	char			st_op;
	char			*word;

	type = NUM;
	word_size = compute_word_size(lexer, &type, &st_op, last_token);
	if (type == NUM && ((last_token && (last_token->type == FRED ||
	last_token->type == BRED)) || ((lexer->line)[word_size] && \
	((lexer->line)[word_size] == '<' || (lexer->line)[word_size] == '>'))))
		type = FD;
	else
		find_type(lexer, last_token, st_op, &type);
	if (!(word = get_word(lexer->line, word_size)))
	{
		lexer->line += word_size;
		return (NULL);
	}
	token = new_token(lexer, type, word, lexer->blank);
	free(word);
	lexer->line += word_size - ft_strlen(VAL);
	return (token);
}
