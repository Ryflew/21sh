/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_tokens.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/10 20:27:45 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/10/10 20:27:45 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

char	is_string_op(char c)
{
	if (c == '\'' || c == '"')
		return (1);
	return (0);
}

char	is_operator(char c, char c2)
{
	if (c == '>' || c == '<' || (c == '&' && c2 == '&') || c == '|' || \
		c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || \
		c == '}' || c == ';' || c == '*' || c == '?' || c == '`' || \
		c == '$' || c == '=')
		return (1);
	return (0);
}

char	is_glob_token(e_token type)
{
	if (type == S_WILDCARD || type == Q_WILDCARD || type == E_WILDCARD
		|| type == LBKT || type == RBKT || type == LBRC || type == RBRC)
		return (1);
	return (0);
}
