/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_brackets.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/14 01:55:53 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/12/09 20:28:04 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "tosh.h"

char		check_quot_brackets2(char *str, char *op, int i, int *j)
{
	if (str[i] == '(')
		op[(*j)++] = str[i];
	else if (*j > 0 && (str[i] == ')' && op[*j - 1] != '('))
		return (1);
	else if (*j > 0 && ((str[i] == ')' && op[*j -1 ] == '(')))
		op[--(*j)] = 0;
	return (0);
}

char		check_quot(char *str, char *op, int *i, int *j)
{
	char	c;

	c = str[*i];
	if ((*i > 0 && str[*i - 1] == '\\') || \
		(*i > 1 && ((str[*i - 1] == '&' && str[*i - 2] == '&') || \
		(str[*i - 1] == '|' && str[*i - 2] == '|'))))
		return (0);
	if (c != '|')
	{
		while (str[++*i])
		{
			if (str[*i] == c && (c == '"' || c == '`') && \
				*i >= 1 && str[*i - 1] == '\\')
			{
				if (*i >= 2 && str[*i - 2] == '\\')
					return (0);
				continue ;
			}
			else if (str[*i] == c)
				return (0);
		}
	}
	op[(*j)++] = c;
	return (1);
}

char		check_special_operator(char *str, int i, int *j, char *op)
{
	if (*j == 0 && ((i >= 1 && str[i - 1] == '\\') ||
		(i >= 2 && ((str[i - 1] == '&' && str[i - 2] == '&') ||
		(str[i - 1] == '|' && str[i - 2] == '|')))))
	{
		if (str[i - 1] == '&')
			op[(*j)++] = '&';
		else if (str[i - 1] == '|')
			op[(*j)++] = 'o';
		else
			op[(*j)++] = '\\';
		return (1);
	}
	return (0);
}

static char	check_quot_brackets3(char *str, char *op, int i, int *j)
{
	char	c;

	c = *j > 0 ? op[*j - 1] : 0;
	if ((!c || c == '(' || (c == '"' && str[i] == '`')) && \
		(str[i] == '"' || str[i] == '\'' || str[i] == '`' || str[i] == '('))
	{
		op[(*j)++] = str[i];
		return (1);
	}
	else if ((c == '`' && str[i] == '`') || \
			(c == '"' && str[i] == '"') || \
			(c == '\'' && str[i] == '\'') || \
			(c == '(' && str[i] == ')'))
	{
		op[--(*j)] = 0;
	}
	return (0);
}

char		*check_quot_brackets(char *str, enum e_state *state)
{
	int		i;
	int		j;
	char	op[ft_strlen(str) + 1];

	j = 0;
	i = -1;
	op[j] = 0;
	while (i < (int)ft_strlen(str) && str[++i])
	{
		if (check_quot_brackets3(str, op, i, &j))
			continue ;
		if (str[i] == '\\' && ++i)
			continue ;
	}
	if (!check_special_operator(str, i, &j, op) && !j)
		*state = COMMAND_RUN;
	else
		*state = ADVANCE_SHELL;
	op[j] = '\0';
	return (ft_strdup(op));
}
