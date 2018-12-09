/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_lbrc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdurst2812 <bdurst2812@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/09 20:36:39 by bdurst2812        #+#    #+#             */
/*   Updated: 2018/12/09 20:41:40 by bdurst2812       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

static char	manage_st_op_or_bs(t_lexer *lx, int i, int *save, t_list *st_ops)
{
	char	st_op;

	st_op = (st_ops) ? *((char*)st_ops->data) : 0;
	if (st_op && ((lx->line)[i] == st_op || (st_op == '[' && \
	(lx->line)[i] == ']' && *save != i - 1) || (st_op == '(' && \
	(lx->line)[i] == ')')))
	{
		ft_pop_node(&st_ops, NULL);
		st_op = (st_ops) ? *((char*)st_ops->data) : 0;
	}
	else if (st_op && ((st_op != '`' && st_op != '(') || (lx->line)[i] != '\\')\
	&& (st_op != '"' || \
	((lx->line)[i] != '\\' && (lx->line)[i] != '`')))
		;
	else if (is_string_op((lx->line)[i]) || (lx->line)[i] == '(' || \
	is_lbkt(lx, i) || (lx->line)[i] == '`')
	{
		if ((lx->line)[i] == '[')
			*save = i;
		ft_node_push_front(&st_ops, &(lx->line)[i]);
		st_op = (lx->line)[i];
	}
	else
		return (0);
	return (1);
}

static char	is_brc_char(t_lexer *lx, int i, char *brc, char *com)
{
	if ((lx->line)[i] == '{')
			++(*brc);
	else if ((lx->line)[i] == '}')
		--(*brc);
	else if ((lx->line)[i] == ',' && *brc == 1)
		++(*com);
	else
		return (0);
	return (1);
}

static void init_is_lbrc_var(char *brc, char *bs, char *com, t_list **st_ops)
{
	*brc = 1;
	*bs = 0;
	*com = 0;
	*st_ops = NULL;
}

char	is_lbrc(t_lexer *lx, int i)
{
	char	brc;
	char	bs;
	char	com;
	int		save;
	t_list	*st_ops;

	if ((lx->line)[i] != '{' || lx->bkt)
		return (0);
	init_is_lbrc_var(&brc, &bs, &com, &st_ops);
	while ((lx->line)[++i])
	{
		if (bs)
			bs = 0;
		else if (manage_st_op_or_bs(lx, i, &save, st_ops))
			;
		else if ((lx->line)[i] == '\\' && !bs)
			bs = 1;
		else if (is_brc_char(lx, i, &brc, &com))
			;
		else if (ft_isblank((lx->line)[i]))
			break ;
		if (!brc)
			break ;
	}
	return ((!brc && com) ? 1 : 0);
}
