/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   second_prompt.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/24 23:43:54 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/03/25 23:52:40 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "21sh.h"

char		check_new_open(char *str, char *op, int *j)
{
	int		i;
	char	error;

	error = 0;
	i = -1;
	while (str[++i] && !error)
	{
		if ((str[i] == '"' || str[i] == '\'' || str[i] == '`') && check_quot(str, op, &i, j))
			break ;
		else
			error = check_quot_brackets2(str, op, i, j);
	}
	if (error)
		return (1);
	return (0);
}

static char		get_good_char(char c)
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

static char		go_to_c(char **str, char c)
{
	int		i;

	i = -1;
	while ((*str)[++i])
	{
		if ((*str)[i] == c)
		{
			*str += i + 1;
			return (1);
		}
	}
	return (0);
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
	char	tmp_op[ft_strlen(str) + ft_strlen(op)];
	int		j;

	j = ft_strlen(op) - 1;
	sync_op(tmp_op, op);
	return (check_new_open(str, op, &j));
}

void	treat_second_prompt(char *string, char **op, e_state *state)
{
	int		j;
	char	*str;
	char	new_op[ft_strlen(string) + ft_strlen(*op)];

	str = string;
	j = ft_strlen(*op) - 1;
	ft_bzero(new_op, ft_strlen(string) + ft_strlen(*op));
	if (check_bad_bracket(string, *op))
	{
		free(*op);
		*state = BRACKET_ERROR;
		return ;
	}
	sync_op(new_op, *op);
	while (*str)
	{
		if (new_op[j] == '|' && !(new_op[j--] = '\0'))
			return ;
		else if (go_to_c(&str, get_good_char(new_op[j])))
		{
			new_op[j--] = '\0';
			continue ;
		}
		else
		{
			if (j == -1 || (new_op[j] != '\'' && new_op[j] != '"' && new_op[j] != '`'))
				if (check_new_open(str, new_op, &j))
					*state = BRACKET_ERROR;
			break ;
		}
	}
	new_op[++j] = '\0';
	free(*op);
	*op = ft_strdup(new_op);
}
