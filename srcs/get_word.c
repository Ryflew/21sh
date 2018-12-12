/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_word.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdurst2812 <bdurst2812@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/10 20:25:56 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/12/12 02:42:25 by bdurst2812       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

static char	*fill_word(t_lexer *lx, char *strf, char const *s, size_t len)
{
	size_t	i;
	char	st_op;
	size_t	new_len;
	char	bs;
	char	c_op;

	i = -1;
	new_len = 0;
	st_op = 0;
	c_op = (lx->st_ops) ? *((char*)lx->st_ops->data) : 0;
	bs = 0;
	while (++i < len)
	{
		if (!bs && is_string_op(s[i]) && !st_op && c_op != '`' && c_op != '(')
			st_op = s[i];
		else if (s[i] == st_op && !bs)
			st_op = 0;
		else if (bs || c_op == '[' || (s[i] != st_op && \
		(s[i] != '\\' || st_op == '\'')))
			strf[new_len++] = s[i];
		bs = (s[i] == '\\' && st_op != '\'' && !bs) ? 1 : 0;
	}
	strf[new_len] = '\0';
	return (strf);
}

void		compute_len(t_lexer *lx, char const *s, size_t len, size_t *new_len)
{
	char	bs;
	char	st_op;
	size_t	i;
	char	c_op;

	bs = 0;
	st_op = 0;
	i = -1;
	c_op = (lx->st_ops) ? *((char*)lx->st_ops->data) : 0;
	while (++i < len)
	{
		if (!bs && is_string_op(st_op) && !st_op && c_op != '`' && c_op != '(')
			st_op = s[i];
		else if (s[i] == st_op && !bs)
			st_op = 0;
		else if (bs || c_op == '[' || (s[i] != st_op && \
		(s[i] != '\\' || st_op == '\'')))
			++(*new_len);
		bs = (s[i] == '\\' && st_op != '\'' && !bs) ? 1 : 0;
	}
}

char		*get_word(t_lexer *lx, char const *s, size_t len)
{
	char	*strf;
	size_t	new_len;

	if (!s)
		return (NULL);
	new_len = 0;
	compute_len(lx, s, len, &new_len);
	if (new_len && (strf = (char*)malloc(sizeof(char) * (new_len + 1))))
		return (fill_word(lx, strf, s, len));
	return (NULL);
}
