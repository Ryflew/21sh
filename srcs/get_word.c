/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_word.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/10 20:25:56 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/10/10 20:25:56 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

static char	*fill_word(char *strf, char const *s, size_t len)
{
	size_t	i;
	char	str_op;
	size_t	new_len;
	char	bs;

	i = -1;
	new_len = 0;
	str_op = 0;
	bs = 0;
	while (++i < len)
	{
		if (!bs && is_string_op(s[i]) && !str_op)
			str_op = s[i];
		else if (s[i] == str_op && !bs)
			str_op = 0;
		else if (bs || (s[i] != str_op && (s[i] != '\\' || str_op == '\'')))
			strf[new_len++] = s[i];
		if (s[i] == '\\' && str_op != '\'' && !bs)
			bs = 1;
		else
			bs = 0;
	}
	strf[new_len] = '\0';
	return (strf);
}

char		*get_word(char const *s, size_t len)
{
	char	*strf;
	size_t	i;
	char	str_op;
	size_t	new_len;
	char	bs;

	if (!s)
		return (NULL);
	i = -1;
	new_len = 0;
	str_op = 0;
	bs = 0;
	while (++i < len)
	{
		if (!bs && is_string_op(s[i]) && !str_op)
			str_op = s[i];
		else if (s[i] == str_op && !bs)
			str_op = 0;
		else if (bs || (s[i] != str_op && (s[i] != '\\' || str_op == '\'')))
			++new_len;
		bs = (s[i] == '\\' && str_op != '\'' && !bs) ? 1 : 0;
	}
	if (new_len && (strf = (char*)malloc(sizeof(char) * (new_len + 1))))
		return (fill_word(strf, s, len));
	return (NULL);
}
