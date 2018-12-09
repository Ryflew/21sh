/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   isnt_brc_or_bkt.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdurst2812 <bdurst2812@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/29 14:57:30 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/12/09 20:41:54 by bdurst2812       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

char	is_rbkt(t_lexer *lexer, char c, int i, t_token *l_tk)
{
	return (c == ']' && (lexer->bkt && i && l_tk && (l_tk->type == LBKT || \
	l_tk->type == E_WILDCARD))) ? 1 : 0;
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
			break ;
		}
	if (bkt)
		return (0);
	return (1);
}

char	is_rbrc(t_lexer *lexer, char c)
{
	return (c == '}' && lexer->brc) ? 1 : 0;
}

char	is_dot_or_slash(char c, t_glob glob)
{
	return (((c == '.' && !glob.hidden_f_d) || \
	(c == '/' && !glob.dir)) ? 1 : 0);
}
