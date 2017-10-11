/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_brackets.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/14 01:55:53 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/10/11 14:54:07 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "tosh.h"

char		check_quot_brackets2(char *str, char *op, int i, int *j)
{
	if (str[i] == '(')
		op[++*j] = str[i];
	else if (*j > -1 && (str[i] == ')' && op[*j] != '('))
		return (1);
	else if (*j > -1 && ((str[i] == ')' && op[*j] == '(')))
		op[(*j)--] = 0;
	return (0);
}

char		check_quot(char *str, char *op, int *i, int *j)
{
	char	c;

	c = str[*i];
	if (!(c == '|' && !str[*i + 1]))
	{
		while (str[++*i])
		{
			if (str[*i] == c && c == '"' && *i >= 1 && str[*i - 1] == '\\')
			{
				if (*i >= 2 && str[*i - 2] == '\\')
					return (0);
				continue ;
			}
			else if (str[*i] == c)
				return (0);
		}
	}
	op[++*j] = c;
	return (1);
}

char		*check_quot_brackets(char *str, enum e_state *state)
{
	int		i;
	int		j;
	char	op[ft_strlen(str)];

	j = -1;
	i = -1;
	while (str[++i])
	{
		if ((str[i] == '"' || str[i] == '\'' || str[i] == '`' || \
			(str[i] == '|' && !str[i + 1])) && check_quot(str, op, &i, &j))
			break ;
		else
		{
			if (check_quot_brackets2(str, op, i, &j) && \
				(*state = BRACKET_ERROR) == BRACKET_ERROR)
				return (NULL);
		}
	}
	if (j == -1)
		*state = COMMAND_RUN;
	else
		*state = ADVANCE_SHELL;
	op[++j] = '\0';
	return (ft_strdup(op));
}
