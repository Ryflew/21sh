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

static int	is_e_wildcard(char *s1, char *s2, t_list *lexems)
{
	t_token	*token;

	token = (t_token*)lexems->next->data;
	s2 = VAL;
	while (*(s2))
		if (*s1 != '\0' && *s2 != '\0' && *s2++ == *s1)
			return (0);
	return (nmatch(s1 + 1, NULL, lexems->next->next->next));
}

static int	is_l_bkt(char *s1, char *s2, t_list *lexems)
{
	t_token	*token;

	token = (t_token*)lexems->next->data;
	s2 = VAL;
	while (*(s2))
		if (*s1 != '\0' && *s2 != '\0' && *s2++ == *s1)
			return (nmatch(s1 + 1, NULL, lexems->next->next->next));
	return (0);
}

static int	is_expr(char *s1, char *s2, t_list *lexems, t_token *token)
{
	if (!s2)
		s2 = VAL;
	if (*s1 != '\0' && *s2 != '\0' && *s2 == *s1)
		return (nmatch(s1 + 1, s2 + 1, lexems));
	else if (!*s2)
		return (nmatch(s1, NULL, lexems->next));
	else if (*s1 == '\0' && *s2 == '/')
		return (1);
	return (0);
}

int			nmatch(char *s1, char *s2, t_list *lexems)
{
	t_token	*token;

	token = (t_token*)lexems->data;
	if (TYPE == S_WILDCARD && *s1 == '\0')
		return (nmatch(s1, NULL, lexems->next));
	if (TYPE == S_WILDCARD && *s1 != '\0')
		return (nmatch(s1 + 1, NULL, lexems) + nmatch(s1, NULL, lexems->next));
	if (*s1 == '\0' && (TYPE == END_EXPR || (s2 && *s2 == '/')))
		return (1);
	if (TYPE == E_WILDCARD && *s1 != '\0' && lexems->next)
		return (is_e_wildcard(s1, s2, lexems));
	if (TYPE == LBKT && *s1 != '\0' && lexems->next)
		return (is_l_bkt(s1, s2, lexems));
	if (TYPE == Q_WILDCARD && *s1 != '\0')
		return (nmatch(s1 + 1, NULL, lexems->next));
	if (TYPE == EXPR && TYPE != END_EXPR)
		return (is_expr(s1, s2, lexems, token));
	return (0);
}
