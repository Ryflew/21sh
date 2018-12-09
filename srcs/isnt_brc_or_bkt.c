/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   isnt_brc_or_bkt.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/29 14:57:30 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/11/29 14:57:37 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

char	is_rbkt(t_lexer *lexer, char c, int i, t_token *l_tk)
{
	return (c == ']' && (lexer->bkt && i && l_tk && (l_tk->type == LBKT || l_tk->type == E_WILDCARD))) ? 1 : 0;
}

char	is_lbkt(t_lexer *lexer, int i)
{
	char	bkt;
	char	save;

	if ((lexer->line)[i] == '[' && lexer->bkt)
		return (1);
	else if ((lexer->line)[i] != '[')
		return (0);
	if ((lexer->line)[i + 1] == '^' || (lexer->line)[i + 1] == '!')
		++i;
	bkt = 1;
	save = i;
	while ((lexer->line)[++i])
		if ((lexer->line)[i] == ']' && save != i - 1)
		{
			bkt = 0;
			break;
		}
	if (bkt)
		return (0);
	return (1);
}

char	is_rbrc(t_lexer *lexer, char c)
{
	return (c == '}' && lexer->brc) ? 1 : 0;
}

char	is_lbrc(t_lexer *lx, int i)
{
	char	brc;
	char	bs;
	char	com;
	char 	st_op;
	int		save;
	t_list	*st_ops;

	if ((lx->line)[i] == '{' && lx->bkt)
		return (0);
	else if ((lx->line)[i] != '{')
		return (0);
	brc = 1;
	bs = 0;
	com = 0;
	st_op = 0;
	st_ops = NULL;
	while ((lx->line)[++i])
	{
		if (bs)
			bs = 0;
		else if (st_op && ((lx->line)[i] == st_op || (st_op == '[' && \
		(lx->line)[i] == ']' && save != i - 1) || (st_op == '(' && (lx->line)[i] == ')')))
		{
			ft_pop_node(&st_ops, NULL);
			st_op = (st_ops) ? *((char*)st_ops->data) : 0;
		}
		else if (st_op && ((st_op != '`' && st_op != '(') || (lx->line)[i] != '\\') && (st_op != '"' || \
		((lx->line)[i] != '\\' && (lx->line)[i] != '`')))
			continue;
		else if ((lx->line)[i] == '\\' && !bs)
			bs = 1;
		else if (is_string_op((lx->line)[i]) || (lx->line)[i] == '(' || (lx->line)[i] == '[' || (lx->line)[i] == '`')
		{
			if ((lx->line)[i] == '[')
				save = i;
			ft_node_push_front(&st_ops, &(lx->line)[i]);
			st_op = (lx->line)[i];
		}
		else if ((lx->line)[i] == '{')
			++brc;
		else if ((lx->line)[i] == '}')
		{
			--brc;
			if (!brc)
				break;
		}
		else if ((lx->line)[i] == ',' && brc == 1)
			++com;
		else if (ft_isblank((lx->line)[i]))
			break;
	}
	if (!brc && com)
		return (1);
	return (0);
}

char	is_dot_or_slash(char c, t_glob glob)
{
	return (((c == '.' && !glob.hidden_f_d) || \
	(c == '/' && !glob.dir)) ? 1 : 0);
}
