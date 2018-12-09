/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compute_word_size.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdurst2812 <bdurst2812@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/09 20:36:47 by bdurst2812        #+#    #+#             */
/*   Updated: 2018/12/09 20:49:28 by bdurst2812       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

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

static char	is_special_char(t_lexer *lx, int i, t_token *l_tk)
{
	if (is_glob_char(lx, (lx->line)[i], i) || ((lx->line)[i] == '.' \
	&& lx->range_brc))
		return (1);
	else if (ft_isblank((lx->line)[i]))
		return (1);
	else if (is_operator((lx->line)[i], (lx->line)[i + 1]))
		return (1);
	else if (is_var_op((lx->line)[i], (lx->line)[i + 1]))
		return (1);
	else if (is_equal(lx, (lx->line)[i], i))
		return (1);
	else if (isnt_var_val(l_tk, (lx->line)[i]))
		return (1);
	else if (is_num_range_expr(lx, i))
		return (1);
	return (0);
}

static char	manage_bs_and_st_op(t_lexer *lx, int i, char *bs, char *st_op)
{
	if (*st_op && ((lx->line)[i] == *st_op || (*st_op == '[' && \
	(lx->line)[i] == ']' && i) || (*st_op == '(' && (lx->line)[i] == ')')))
	{
		if (!is_string_op(*st_op))
			return (1);
		ft_pop_node(&lx->st_ops, NULL);
		*st_op = (lx->st_ops) ? *((char*)lx->st_ops->data) : 0;
	}
	else if (*st_op && ((*st_op != '`' && *st_op != '(') || \
	(lx->line)[i] != '\\') && (*st_op != '"' || \
	((lx->line)[i] != '\\' && (lx->line)[i] != '`' && (lx->line)[i] != '$')))
		;
	else if ((lx->line)[i] == '\\' && !*bs)
		*bs = 1;
	else if (is_string_op((lx->line)[i]) || (lx->line)[i] == '(' || \
	is_lbkt(lx, i) || (lx->line)[i] == '`')
	{
		if (!is_string_op((lx->line)[i]))
			return (1);
		ft_node_push_front(&lx->st_ops, &(lx->line)[i]);
		*st_op = (lx->st_ops) ? *((char*)lx->st_ops->data) : 0;
	}
	else
		return (-1);
	return (0);
}

static void	init_var(char *bs, int *i)
{
	*bs = 0;
	*i = 0;
}

int			compute_word_size(t_lexer *lx, enum e_token *type, char *st_op, \
							t_token *l_tk)
{
	char	bs;
	int		i;
	char	ret;

	init_var(&bs, &i);
	*st_op = (lx->st_ops) ? *((char*)lx->st_ops->data) : 0;
	while ((lx->line)[i])
	{
		if (bs)
			bs = 0;
		else if ((ret = manage_bs_and_st_op(lx, i, &bs, st_op)) == 1)
			break ;
		else if (!ret)
			;
		else if (is_regular_char_in_here(lx, (lx->line)[i]))
			return (0);
		else if (is_special_char(lx, i, l_tk))
			break ;
		if_its_word((lx->line)[i], type, i);
		if (is_start_range_expr(lx, l_tk, lx->line, ++i))
			return (i == 1) ? i : --i;
		if (l_tk && l_tk->type == DASH && i == 1)
			return (i);
	}
	return (i);
}
