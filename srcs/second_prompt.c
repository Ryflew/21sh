/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   second_prompt.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/24 23:43:54 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/12/09 20:22:27 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "tosh.h"

static char	get_good_char(char c)
{
	if (c == '(')
		return (')');
	else if (c == '{')
		return ('}');
	else if (c == '[')
		return (']');
	else
		return (c);
}

static void	sync_op(char *new, char *old)
{
	int		i;

	i = -1;
	while (old[++i])
		new[i] = old[i];
}

static char	check_bad_bracket(char *str, char *op)
{
	char	tmp_op[ft_strlen(str) + ft_strlen(op) + 1];
	int		j;

	j = ft_strlen(op) - 1;
	sync_op(tmp_op, op);
	return (check_new_open(str, tmp_op, &j));
}

static void	continue_treat(char *str, int *j, char *new_op, enum e_state *state)
{
	while (*str)
	{
		if (*j > 0 && new_op[*j - 1] == '|' && !(new_op[--(*j)] = '\0'))
		{
			if (check_new_open(str, new_op, j))
				*state = BRACKET_ERROR;
			return ;
		}
		else if (*j > 0 && go_to_c(&str, get_good_char(new_op[*j - 1])))
		{
			new_op[--(*j)] = '\0';
			continue ;
		}
		else
		{
			if (!*j || (new_op[*j - 1] != '\'' && \
				new_op[*j - 1] != '"' && new_op[*j - 1] != '`'))
				if (check_new_open(str, new_op, j))
					*state = BRACKET_ERROR;
			return ;
		}
	}
}

void		treat_second_prompt(char *string, char **op, enum e_state *state)
{
	int		j;
	char	*str;
	char	new_op[ft_strlen(string) + ft_strlen(*op) + 1];

	str = string;
	j = ft_strlen(*op);
	ft_bzero(new_op, ft_strlen(string) + ft_strlen(*op));
	if (check_bad_bracket(string, *op))
	{
		free(*op);
		*state = BRACKET_ERROR;
		return ;
	}
	sync_op(new_op, *op);
	if (ft_strlen(*op) > 0 && \
		(*(*op + ft_strlen(*op) - 1) == '\\' || \
		*(*op + ft_strlen(*op) - 1) == '&' || \
		*(*op + ft_strlen(*op) - 1) == 'o'))
		new_op[--j] = '\0';
	continue_treat(str, &j, new_op, state);
	check_special_operator(str, ft_strlen(str), &j, new_op);
	new_op[j] = '\0';
	free(*op);
	*op = ft_strdup(new_op);
}
