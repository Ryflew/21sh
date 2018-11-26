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

char isnt_var_val(t_token *last_token, char c)
{
	return (last_token && last_token->type == VAR_OP && !ft_isalnum(c)) ? 1 : 0;
}

char is_bs_or_bqt_or_par(t_lexer *lexer, char bs, char c)
{
	return (bs || (lexer->bqt && (c != '`' || lexer->bkt)) || (lexer->par && (c != ')' || lexer->bkt))) ? 1 : 0;
}

char isnt_comma(t_lexer *lexer, char c)
{
	return (c == ',' && (!lexer->brc || lexer->bkt)) ? 1 : 0;
}

char is_regular_char_in_st_op(char st_op, char c)
{
	return (st_op && ((c != '`' && c != '$') || st_op == '\'')) ? 1 : 0;
}

char isnt_var_op(char c1, char c2)
{
	return (c1 == '$' && !ft_isalnum(c2)) ? 1 : 0;
}

char isnt_equal(t_lexer *lexer, char c, int i)
{
	return (c == '=' && (!i || ft_isblank((lexer->line)[i - 1]))) ? 1 : 0;
}

char is_regular_char_in_here(t_lexer *lexer, char c)
{
	return (lexer->her && c != '`' && c != '$') ? 1 : 0;
}

char is_start_range_expr(t_lexer *lexer, t_token *last_token, char *line, int i)
{
	if (lexer->bkt && last_token->type != DASH && line[i] && line[i] == '-' \
	&& line[i + 1] && line[i + 1] != ']' && line[i - 1] < line[i + 1])
		return (1);
	return (0);
}

char isnt_rbkt(t_lexer *lexer, char c, int i)
{
	return (c == ']' && (!lexer->bkt || !i)) ? 1 : 0;
}

char isnt_lbkt(t_lexer *lexer, char c)
{
	return (c == '[' && lexer->bkt) ? 1 : 0;
}

char isnt_rbrc(t_lexer *lexer, char c)
{
	return (c == '}' && !lexer->brc) ? 1 : 0;
}

char isnt_lbrc(t_lexer *lexer, char c)
{
	return (c == '{' && lexer->bkt) ? 1 : 0;
}

char isnt_s_wildcard(t_lexer *lexer, char c)
{
	return (c == '*' && lexer->bkt) ? 1 : 0;
}

char isnt_q_wildcard(t_lexer *lexer, char c)
{
	return (c == '?' && lexer->bkt) ? 1 : 0;
}

char isnt_glob_char(t_lexer *lx, char c, int i)
{
	if (isnt_lbrc(lx, c) || isnt_rbrc(lx, c) || isnt_comma(lx, c) || isnt_lbkt(lx, c) \
	|| isnt_rbkt(lx, c, i) || isnt_s_wildcard(lx, c) || isnt_q_wildcard(lx, c))
		return (1);
	return (0);
}

int			compute_word_size(t_lexer *lx, enum e_token *type, char *st_op,
								t_token *l_tk)
{
	char	bs;
	int		i;

	bs = 0;
	*st_op = 0;
	i = 0;
	while ((lx->line)[i] && (is_bs_or_bqt_or_par(lx, bs, (lx->line)[i]) \
	|| (!ft_isblank((lx->line)[i]) && (!is_operator((lx->line)[i], (lx->line)[i + 1])
	|| isnt_glob_char(lx, (lx->line)[i], i) \
	|| isnt_var_op((lx->line)[i], (lx->line)[i + 1]) \
	|| isnt_equal(lx, (lx->line)[i], i) || isnt_var_val(l_tk, (lx->line)[i]))) \
	|| is_regular_char_in_st_op(*st_op, (lx->line)[i]) \
	|| is_regular_char_in_here(lx, (lx->line)[i]))) {
		if ((lx->line)[i] == '\\' && lx->bqt)
			bs = 1;
		if_its_word((lx->line)[i++], &bs, st_op, type);
		if (is_start_range_expr(lx, l_tk, lx->line, i))
			return (i == 1) ? i : --i;
		if (l_tk && l_tk->type == DASH && i == 1)
			return (i);
	}
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
	if (isnt_here_and_bqt(lx) && l_tk && is_glob_token(l_tk->type)\
		&& !lx->blank)
	{
		if (*lx->line && is_start_range_expr(lx, l_tk, lx->line, 1))
			*type = START_RANGE_EXPR;
		else if (l_tk && l_tk->type == DASH)
			*type = END_RANGE_EXPR;
		else if (lx->bkt)
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
