/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_brackets2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/20 18:44:24 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/03/20 21:28:48 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "21sh.h"

char		check_quot_brackets2(char *str, char *op, int i, int *j)
{
	if (str[i] == '(' || str[i] == '{' || str[i] == '[')
		op[++*j] = str[i];
	else if (*j > -1 && ((str[i] == ')' && op[*j] != '(') || \
		(str[i] == '}' && op[*j] != '{') || (str[i] == ']' && op[*j] != '[')))
		return (1);
	else if (*j > -1 && ((str[i] == ')' && op[*j] == '(') || \
		(str[i] == '}' && op[*j] == '{') || (str[i] == ']' && op[*j] == '[')))
		op[(*j)--] = 0;
	return (0);
}

char		check_new_open(char *str, char *op, int *j)
{
	int		i;
	char	error;

	error = 0;
	i = -1;
	while (str[++i])
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

char		get_good_char(char c)
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

char		go_to_c(char **str, char **tmp, char c)
{
	int		i;
	char	*temp;
	char	*temp2;

	i = -1;
	while ((*str)[++i])
	{
		if ((*str)[i] == c)
		{
			temp = ft_strsub(*str, 0, i + 1);
			temp2 = *tmp;
			*tmp = ft_strjoin(*tmp, temp);
			free(temp);
			free(temp2);
			*str += i + 1;
			return (1);
		}
	}
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
			if (str[*i] == c)
				return (0);
		}
	}
	op[++*j] = c;
	return (1);
}
