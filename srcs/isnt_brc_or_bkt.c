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

char	isnt_rbkt(t_lexer *lexer, char c, int i)
{
	return (c == ']' && (!lexer->bkt || !i)) ? 1 : 0;
}

char	isnt_lbkt(t_lexer *lexer, char c)
{
	return (c == '[' && lexer->bkt) ? 1 : 0;
}

char	isnt_rbrc(t_lexer *lexer, char c)
{
	return (c == '}' && !lexer->brc) ? 1 : 0;
}

char	isnt_lbrc(t_lexer *lexer, char c)
{
	return (c == '{' && lexer->bkt) ? 1 : 0;
}

char	is_dot_or_slash(char c, t_glob glob)
{
	return (((c == '.' && !glob.hidden_f_d) || \
	(c == '/' && !glob.dir)) ? 1 : 0);
}
