/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_string.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/10 20:27:49 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/11/29 15:00:02 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

// static void	if_its_word(char c, char *bs, char *st_op, enum e_token *type, int i)
// {
// 	if (!ft_isdigit(c))
// 	{
// 		if (!i && c == '-')
// 			*type = NEG_NUM;
// 		else if (!ft_isalnum(c))
// 			*type = WORD;
// 		else if (*type != WORD)
// 			*type = ASCII_WORD;
// 	}
// 	if (is_string_op(c) && !*st_op)
// 		*st_op = c;
// 	else if (c == *st_op)
// 		*st_op = 0;
// 	if (c == '\\' && *st_op != '\'' && !*bs)
// 		*bs = 1;
// 	else
// 		*bs = 0;
// }

static void	if_its_word(char c, enum e_token *type, int i)
{
	if (!ft_isdigit(c))
	{
		if (!i && c == '-')
			*type = NEG_NUM;
		else if (!ft_isalnum(c))
			*type = WORD;
		else if (*type != WORD)
			*type = ASCII_WORD;
	}
}

char		is_num_range_expr(t_lexer *lx, int i)
{
	int	saveIndex;

	if ((lx->line)[i] == '{' && lx->bkt)
		return (0);
	else if ((lx->line)[i] != '{')
		return (0);
	saveIndex = ++i;
	while ((lx->line)[i] && (ft_isdigit((lx->line)[i]) || (i == saveIndex && (lx->line)[i] == '-')))
			++i;
	if (!i)
		return (0);
	if ((lx->line)[i++] == '.')
	{
		if ((lx->line)[i] && (lx->line)[i++] != '.')
			return (0);
		saveIndex = i;
		while ((lx->line)[i] && (ft_isdigit((lx->line)[i]) || (i == saveIndex && (lx->line)[i] == '-')))
			++i;
		if ((lx->line)[i] == '}' && i > saveIndex)
		{
			lx->range_brc = 1;
			return (1);
		}
	}
	return (0);
}

int			compute_word_size(t_lexer *lx, enum e_token *type, char *st_op, t_token *l_tk)
{
	char	bs;
	int		i;

	bs = 0;
	*st_op = (lx->st_ops) ? *((char*)lx->st_ops->data) : 0;
	i = 0;
	while ((lx->line)[i])
	{
		if (bs)
			bs = 0;
		else if (*st_op && ((lx->line)[i] == *st_op || (*st_op == '[' && \
		(lx->line)[i] == ']' && i) || (*st_op == '(' && (lx->line)[i] == ')')))
		{
			if (!is_string_op(*st_op))
				break;
			ft_pop_node(&lx->st_ops, NULL);
			*st_op = (lx->st_ops) ? *((char*)lx->st_ops->data) : 0;
		}
		else if (*st_op && ((*st_op != '`' || ((lx->line)[i] != '\'' && \
		(lx->line)[i] != '"' && (lx->line)[i] != '$' && \
		(lx->line)[i] != '\\' && (lx->line)[i] != '(')) && (*st_op != '(' || ((lx->line)[i] != '\'' && \
		(lx->line)[i] != '"' && (lx->line)[i] != '$' && \
		(lx->line)[i] != '\\' && (lx->line)[i] != '`')) && (*st_op != '"' || \
		((lx->line)[i] != '\\' && (lx->line)[i] != '`' && (lx->line)[i] != '$'))))
			;
		else if ((lx->line)[i] == '\\' && !bs)
			bs = 1;
		else if (is_string_op((lx->line)[i]) || (lx->line)[i] == '(' || (lx->line)[i] == '[' || (lx->line)[i] == '`')
		{
			if (!is_string_op((lx->line)[i]))
				break;
			ft_node_push_front(&lx->st_ops, &(lx->line)[i]);
			*st_op = (lx->st_ops) ? *((char*)lx->st_ops->data) : 0;
		}
		else if (is_regular_char_in_here(lx, (lx->line)[i]))
			;
		else if (is_glob_char(lx, (lx->line)[i], i) || ((lx->line)[i] == '.' && lx->range_brc))
			break;
		else if (ft_isblank((lx->line)[i]))
			break;
		else if (is_operator((lx->line)[i], (lx->line)[i + 1]))
			break;
		else if (is_var_op((lx->line)[i], (lx->line)[i + 1]))
			break;
		else if (is_equal(lx, (lx->line)[i], i))
			break;
		else if (isnt_var_val(l_tk, (lx->line)[i]))
			break;
		else if (is_num_range_expr(lx, i))
			break ;
		if_its_word((lx->line)[i], type, i);
		if (is_start_range_expr(lx, l_tk, lx->line, ++i))
			return (i == 1) ? i : --i;
		if (l_tk && l_tk->type == DASH && i == 1)
			return (i);
	}
	return (i);
}

// int			compute_word_size(t_lexer *lx, enum e_token *type, char *st_op,
// 								t_token *l_tk)
// {
// 	char	bs;
// 	int		i;
// 	t_list	*st_ops;

// 	bs = 0;
// 	st_ops = NULL;
// 	*st_op = 0;
// 	i = 0;
// while ((lx->line)[i] && (is_bs_or_bqt_or_par(lx, bs, (lx->line)[i]) \
// 	|| (!ft_isblank((lx->line)[i]) \
// 	&& (!is_operator((lx->line)[i], (lx->line)[i + 1]) \
// 	|| isnt_glob_char(lx, (lx->line)[i], i) || ((lx->line)[i] == '.' && !lx->range_brc) \
// 	|| isnt_var_op(lx, (lx->line)[i], (lx->line)[i + 1]) \
// 	|| isnt_equal(lx, (lx->line)[i], i) || isnt_var_val(l_tk, (lx->line)[i]))) \
// 	|| is_regular_char_in_st_op(*st_op, (lx->line)[i]) \
// 	|| is_regular_char_in_here(lx, (lx->line)[i])))
// 	{
// 		// if ((lx->line)[i] == '\\' && lx->bqt)
// 		// 	bs = 1;
// 		if (is_num_range_expr(lx, i))
// 			return (i);
// 		if_its_word((lx->line)[i], &bs, st_op, type, i);
// 		if (is_start_range_expr(lx, l_tk, lx->line, ++i))
// 			return (i == 1) ? i : --i;
// 		if (l_tk && l_tk->type == DASH && i == 1)
// 			return (i);
// 	}
// 	return (i);
// }

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
	else if (isnt_here_and_bqt(lx) && l_tk && l_tk->type == EQUAL \
			&& !lx->blank && !lx->blank)
		*type = (*lx->line == '~') ? TILD_VAR_WORD : VAR_WORD;
	if (lx->range_brc && (*type == NUM || *type == NEG_NUM))
		*type = NUM_EXPR;
	else if (isnt_here_and_bqt(lx) && l_tk && is_glob_token(l_tk->type)\
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
