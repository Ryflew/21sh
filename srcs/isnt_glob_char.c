/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   isnt_glob_char.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/29 14:56:05 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/11/29 14:57:11 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

static char	is_s_wildcard(char c)
{
	return (c == '*') ? 1 : 0;
}

static char	is_q_wildcard(char c)
{
	return (c == '?') ? 1 : 0;
}

static char	is_comma(t_lexer *lexer, char c)
{
	return (c == ',' && lexer->brc) ? 1 : 0;
}

char		is_glob_char(t_lexer *lx, char c, int i)
{
	if (is_lbrc(lx, i) || is_rbrc(lx, (lx->line)[i]) || is_lbkt(lx, i) || \
	is_comma(lx, c) || is_s_wildcard(c) || is_q_wildcard(c))
		return (1);
	return (0);
}
