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

static int	is_e_wildcard(char *s1, char *s2, t_list *lexems, t_glob glob)
{
	t_token	*token;

	token = (t_token*)lexems->next->data;
	s2 = VAL;
	glob.type = E_WILDCARD;
	if (TYPE == START_RANGE_EXPR)
		return (nmatch(s1, NULL, lexems->next, glob));
	while (*(s2))
		if (*s1 != '\0' && *s2 != '\0' && *s2++ == *s1)
			return (0);
	return (nmatch(s1 + 1, NULL, lexems->next->next, glob));
}

static int	is_l_bkt(char *s1, char *s2, t_list *lexems, t_glob glob)
{
	t_token	*token;

	token = (t_token*)lexems->next->data;
	s2 = VAL;
	glob.type = LBKT;
	if (TYPE == START_RANGE_EXPR)
		return (nmatch(s1, NULL, lexems->next, glob));
	while (*(s2))
		if (*s1 != '\0' && *s2 != '\0' && *s2++ == *s1)
			return (nmatch(s1 + 1, NULL, lexems->next->next, glob));
	return (0);
}

static int	is_range_expr(t_list *lexems, char *s1, t_glob glob)
{
	t_token	*token;
	char	from;
	char	to;

	token = (t_token*)lexems->data;
	from = *VAL;
	token = (t_token*)lexems->next->next->data;
	to = *VAL;
	if (glob.type == LBKT && *s1 >= from && *s1 <= to)
		return (nmatch(s1 + 1, NULL, lexems->next->next->next, glob));
	else if (glob.type == E_WILDCARD && (*s1 < from || *s1 > to))
		return (nmatch(s1 + 1, NULL, lexems->next->next->next, glob));
	return (0);
}

static int	is_expr(char *s1, char *s2, t_list *lexems, t_glob glob)
{
	t_token			*token;
	
	token = (t_token*)lexems->data;
	if (!s2)
		s2 = VAL;
	if (*s1 == *s2 && *s1 == '/')
	{
		glob.dir = 1;
		glob.hidden_f_d = 0;
	}
	else if (*s1 == *s2 && *s1 == '.')
		glob.hidden_f_d = 1;
	if (*s1 != '\0' && *s2 != '\0' && *s2 == *s1)
		return (nmatch(s1 + 1, s2 + 1, lexems, glob));
	else if (!*s2)
		return (nmatch(s1, NULL, lexems->next, glob));
	else if (*s1 == '\0' && *s2 == '/')
		return (1);
	return (0);
}

int			nmatch(char *s1, char *s2, t_list *lxms, t_glob glob)
{
	t_token			*token;

	token = (t_token*)lxms->data;
	if (TYPE == S_WILDCARD && *s1 == '\0' && !is_dot_or_slash(*s1, glob))
		return (nmatch(s1, NULL, lxms->next, glob));
	if (TYPE == S_WILDCARD && *s1 != '\0' && !is_dot_or_slash(*s1, glob))
		return (nmatch(s1 + 1, NULL, lxms, glob) +
				nmatch(s1, NULL, lxms->next, glob));
	if (*s1 == '\0' && (TYPE == END_EXPR || (s2 && *s2 == '/')))
		return (1);
	if (TYPE == E_WILDCARD && *s1 != '\0' && lxms->next && \
	!is_dot_or_slash(*s1, glob))
		return (is_e_wildcard(s1, s2, lxms, glob));
	if (TYPE == LBKT && *s1 != '\0' && lxms->next && !is_dot_or_slash(*s1, glob))
		return (is_l_bkt(s1, s2, lxms, glob));
	if (TYPE == START_RANGE_EXPR && *s1 != '\0' && !is_dot_or_slash(*s1, glob))
		return (is_range_expr(lxms, s1, glob));
	if (TYPE == Q_WILDCARD && *s1 != '\0' && !is_dot_or_slash(*s1, glob))
		return (nmatch(s1 + 1, NULL, lxms->next, glob));
	if (TYPE == EXPR && TYPE != END_EXPR)
		return (is_expr(s1, s2, lxms, glob));
	glob.type = NONE;
	if (TYPE == RBKT)
		return (nmatch(s1, NULL, lxms->next, glob));
	return (0);
}
