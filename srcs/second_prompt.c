/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   second_prompt.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/24 23:43:54 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/12/11 15:32:50 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "tosh.h"

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

static void	continue_treat(char *str, int *j, char *op, enum e_state *state)
{
	int		i;

	i = -1;
	while (i < (int)ft_strlen(str) && str[++i])
	{
		if (check_quot_brackets3(str, op, i, j))
			continue ;
		if (str[i] == '\\' && ++i)
			continue ;
	}
	if (!check_special_operator(str, i, j, op) && !*j)
		*state = COMMAND_RUN;
	else
		*state = ADVANCE_SHELL;
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
	// check_special_operator(str, ft_strlen(str), &j, new_op);
	new_op[j] = '\0';
	free(*op);
	*op = ft_strdup(new_op);
}
