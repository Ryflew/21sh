/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   completion_builtin2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/09 16:44:44 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/12/09 17:19:32 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

static void	count_builtins2(char *part, int *count, char **arr)
{
	if (!ft_strncmp(part, arr[4], ft_strlen(part)))
		++*count;
	if (!ft_strncmp(part, arr[5], ft_strlen(part)))
		++*count;
	if (!ft_strncmp(part, arr[6], ft_strlen(part)))
		++*count;
	if (!ft_strncmp(part, arr[7], ft_strlen(part)))
		++*count;
}

int			count_builtins(char *part)
{
	char	*arr[8];
	int		count;

	arr[0] = "setenv";
	arr[1] = "unsetenv";
	arr[2] = "exit";
	arr[3] = "history";
	arr[4] = "export";
	arr[5] = "shellvar";
	arr[6] = "help";
	arr[7] = "unset";
	count = 0;
	if (!ft_strncmp(part, arr[0], ft_strlen(part)))
		++count;
	if (!ft_strncmp(part, arr[1], ft_strlen(part)))
		++count;
	if (!ft_strncmp(part, arr[2], ft_strlen(part)))
		++count;
	if (!ft_strncmp(part, arr[3], ft_strlen(part)))
		++count;
	count_builtins2(part, &count, arr);
	return (count);
}
