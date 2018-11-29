/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nmatch.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/11 17:47:20 by bdurst            #+#    #+#             */
/*   Updated: 2018/11/29 15:03:46 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

static int	is_range_expr(t_list **lexems, char *s1)
{
	t_token	*token;
	char	from;
	char	to;

	token = (t_token*)(*lexems)->data;
	from = *VAL;
	*lexems = (*lexems)->next->next;
	token = (t_token*)(*lexems)->data;
	to = *VAL;
	if (*s1 >= from && *s1 <= to)
		return (1);
	return (0);
}

static int	is_e_wildcard(char *s1, char *s2, t_list *lexems, t_glob glob)
{
	t_token	*token;
	char	match;

	token = (t_token*)lexems->data;
	match = 0;
	while (TYPE != RBKT)
	{
		if (!match)
		{
			if (TYPE == START_RANGE_EXPR && is_range_expr(&lexems, s1))
				match = 1;
			else if (TYPE != START_RANGE_EXPR)
			{
				s2 = VAL;
				while (*(s2))
					if (*s1 != '\0' && *s2 != '\0' && *s2++ == *s1)
						match = 1;
			}
		}
		lexems = lexems->next;
		token = (t_token*)lexems->data;
	}
	if (match)
		return (0);
	return (nmatch(s1 + 1, NULL, lexems->next, glob));
}

static int	is_l_bkt(char *s1, char *s2, t_list *lexems, t_glob glob)
{
	t_token	*token;
	char	match;

	token = (t_token*)lexems->data;
	match = 0;
	while (TYPE != RBKT)
	{
		if (!match)
		{
			if (TYPE == START_RANGE_EXPR && is_range_expr(&lexems, s1))
				match = 1;
			else if (TYPE != START_RANGE_EXPR)
			{
				s2 = VAL;
				while (*(s2))
					if (*s1 != '\0' && *s2 != '\0' && *s2++ == *s1)
						match = 1;
			}
		}
		lexems = lexems->next;
		token = (t_token*)lexems->data;
	}
	if (match)
		return (nmatch(s1 + 1, NULL, lexems->next, glob));
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
	{
		return (nmatch(s1 + 1, NULL, lxms, glob) + \
			nmatch(s1, NULL, lxms->next, glob));
	}
	if (*s1 == '\0' && (TYPE == END_EXPR || (s2 && *s2 == '/')))
		return (1);
	if (TYPE == E_WILDCARD && *s1 != '\0' && lxms->next && \
	!is_dot_or_slash(*s1, glob))
		return (is_e_wildcard(s1, s2, lxms->next, glob));
	if (TYPE == LBKT && *s1 != '\0' && lxms->next && \
		!is_dot_or_slash(*s1, glob))
		return (is_l_bkt(s1, s2, lxms->next, glob));
	if (TYPE == Q_WILDCARD && *s1 != '\0' && !is_dot_or_slash(*s1, glob))
		return (nmatch(s1 + 1, NULL, lxms->next, glob));
	if (TYPE == EXPR && TYPE != END_EXPR)
		return (is_expr(s1, s2, lxms, glob));
	return (0);
}
