/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_string.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdurst2812 <bdurst2812@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/10 20:27:49 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/12/09 20:39:50 by bdurst2812       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

char		is_num_range_expr(t_lexer *lx, int i)
{
	int	saveIndex;

	if ((lx->line)[i] != '{' || lx->bkt)
		return (0);
	saveIndex = ++i;
	while ((lx->line)[i] && (ft_isdigit((lx->line)[i]) || \
	(i == saveIndex && (lx->line)[i] == '-')))
			++i;
	if (i && (lx->line)[i++] == '.')
	{
		if ((lx->line)[i] && (lx->line)[i++] != '.')
			return (0);
		saveIndex = i;
		while ((lx->line)[i] && (ft_isdigit((lx->line)[i]) || \
		(i == saveIndex && (lx->line)[i] == '-')))
			++i;
		if ((lx->line)[i] == '}' && i > saveIndex)
		{
			lx->range_brc = 1;
			return (1);
		}
	}
	return (0);
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
	if (lx->range_brc && (*type == NUM || *type == NEG_NUM))
		*type = NUM_EXPR;
	else if (isnt_here_and_bqt(lx) && l_tk && is_glob_token(l_tk->type)\
		&& !lx->blank)
	{
		*type = EXPR;
		if (*lx->line && is_start_range_expr(lx, l_tk, lx->line, 1))
			*type = START_RANGE_EXPR;
		else if (l_tk && l_tk->type == DASH)
			*type = END_RANGE_EXPR;
		else if (lx->bkt)
			*type = BKT_EXPR;
		else if (*type == TILD)
			*type = TILD_EXPR;
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
	if (!(word = get_word(lexer, lexer->line, word_size)))
	{
		lexer->line += word_size;
		return (NULL);
	}
	token = new_token(lexer, type, word, lexer->blank);
	free(word);
	lexer->line += word_size - ft_strlen(VAL);
	return (token);
}
