/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   second_prompt2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/02 02:57:40 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/12/11 15:07:56 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

char		check_new_open(char *str, char *op, int *j)
{
	int		i;
	char	error;

	error = 0;
	i = -1;
	while (str[++i] && !error)
	{
		if ((str[i] == '"' || str[i] == '\'' || str[i] == '`' || \
			(str[i] == '|' && !str[i + 1])) && check_quot(str, op, &i, j))
			break ;
		else
			error = check_quot_brackets2(str, op, i, j);
	}
	if (error)
		return (1);
	return (0);
}

char		go_to_c(char **str, char c, char *op, int j)
{
	int		i;

	i = -1;
	while ((*str)[++i])
	{
		if ((c == '"' && (*str)[i] == c && j > 1 && op[j - 2] == '`'))
			continue ;
		else if ((*str)[i] == c)
		{
			*str += i + 1;
			return (1);
		}
	}
	return (0);
}
