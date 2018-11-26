/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nmatch.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdurst <bdurst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/11 17:47:20 by bdurst            #+#    #+#             */
/*   Updated: 2017/10/11 17:47:20 by bdurst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

static char dot_or_slash(char c)
{
	return ((c == '.' || c == '/') ? 1 : 0);
}

static int	is_e_wildcard(char *s1, char *s2, t_list *lexems)
{
	t_token	*token;

	token = (t_token*)lexems->next->data;
	s2 = VAL;
	if (TYPE == START_RANGE_EXPR)
		return (nmatch(s1, NULL, lexems->next, E_WILDCARD));
	while (*(s2))
		if (*s1 != '\0' && *s2 != '\0' && *s2++ == *s1)
			return (0);
	return (nmatch(s1 + 1, NULL, lexems->next->next, E_WILDCARD));
}

static int	is_l_bkt(char *s1, char *s2, t_list *lexems)
{
	t_token	*token;

	token = (t_token*)lexems->next->data;
	s2 = VAL;
	if (TYPE == START_RANGE_EXPR)
		return (nmatch(s1, NULL, lexems->next, LBKT));
	while (*(s2))
		if (*s1 != '\0' && *s2 != '\0' && *s2++ == *s1)
			return (nmatch(s1 + 1, NULL, lexems->next->next, LBKT));
	return (0);
}

static int	is_range_expr(t_list *lexems, char *s1, enum e_token type)
{
	t_token	*token;
	char	from;
	char	to;

	token = (t_token*)lexems->data;
	from = *VAL;
	token = (t_token*)lexems->next->next->data;
	to = *VAL;
	if (type == LBKT && *s1 >= from && *s1 <= to)
		return (nmatch(s1 + 1, NULL, lexems->next->next->next, type));
	else if (type == E_WILDCARD && (*s1 < from || *s1 > to))
		return (nmatch(s1 + 1, NULL, lexems->next->next->next, type));
	return (0);
}

static int	is_expr(char *s1, char *s2, t_list *lexems, enum e_token type)
{
	t_token			*token;
	
	token = (t_token*)lexems->data;
	if (!s2)
		s2 = VAL;
	if (*s1 != '\0' && *s2 != '\0' && *s2 == *s1)
		return (nmatch(s1 + 1, s2 + 1, lexems, type));
	else if (!*s2)
		return (nmatch(s1, NULL, lexems->next, type));
	else if (*s1 == '\0' && *s2 == '/')
		return (1);
	return (0);
}

static int is_r_bkt(t_list *lexems, char *s1)
{
	return (nmatch(s1, NULL, lexems->next, NONE));
}

int			nmatch(char *s1, char *s2, t_list *lxms, enum e_token type)
{
	t_token			*token;

	token = (t_token*)lxms->data;
	if (TYPE == S_WILDCARD && *s1 == '\0' && !dot_or_slash(*s1))
		return (nmatch(s1, NULL, lxms->next, type));
	if (TYPE == S_WILDCARD && *s1 != '\0' && !dot_or_slash(*s1))
		return (nmatch(s1 + 1, NULL, lxms, type) +
				nmatch(s1, NULL, lxms->next, type));
	if (*s1 == '\0' && (TYPE == END_EXPR || (s2 && *s2 == '/')))
		return (1);
	if (TYPE == E_WILDCARD && *s1 != '\0' && lxms->next && !dot_or_slash(*s1))
		return (is_e_wildcard(s1, s2, lxms));
	if (TYPE == LBKT && *s1 != '\0' && lxms->next && !dot_or_slash(*s1))
		return (is_l_bkt(s1, s2, lxms));
	if (TYPE == START_RANGE_EXPR && *s1 != '\0' && !dot_or_slash(*s1))
		return (is_range_expr(lxms, s1, type));
	if (TYPE == Q_WILDCARD && *s1 != '\0' && !dot_or_slash(*s1))
		return (nmatch(s1 + 1, NULL, lxms->next, type));
	if (TYPE == EXPR && TYPE != END_EXPR)
		return (is_expr(s1, s2, lxms, type));
	if (TYPE == RBKT)
		return (is_r_bkt(lxms, s1));
	return (0);
}
