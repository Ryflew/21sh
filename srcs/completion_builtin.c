/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   completion_builtin.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/08 18:29:06 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/12/08 18:57:01 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

static void	check_builtin2(char **arr, char *part, char *to_add, int *out)
{
	if (!ft_strncmp(part, arr[6], ft_strlen(part)) && ++*out)
		ft_strcpy(to_add, arr[6] + ft_strlen(part));
	if (!ft_strncmp(part, arr[7], ft_strlen(part)) && ++*out)
		ft_strcpy(to_add, arr[7] + ft_strlen(part));
	if (!ft_strncmp(part, arr[3], ft_strlen(part)) && ++*out)
		ft_strcpy(to_add, arr[3] + ft_strlen(part));
	if (!ft_strncmp(part, arr[4], ft_strlen(part)) && ++*out)
		ft_strcpy(to_add, arr[4] + ft_strlen(part));
	if (!ft_strncmp(part, arr[5], ft_strlen(part)) && ++*out)
		ft_strcpy(to_add, arr[5] + ft_strlen(part));
}

int			check_builtin(char *part, char *to_add)
{
	char	*arr[8];
	int		out;

	arr[0] = "setenv";
	arr[1] = "unsetenv";
	arr[2] = "exit";
	arr[3] = "history";
	arr[4] = "export";
	arr[5] = "shellvar";
	arr[6] = "help";
	arr[7] = "unset";
	out = 0;
	if (!ft_strncmp(part, arr[0], ft_strlen(part)) && ++out)
		ft_strcpy(to_add, arr[0] + ft_strlen(part));
	if (!ft_strncmp(part, arr[1], ft_strlen(part)) && ++out)
		ft_strcpy(to_add, arr[1] + ft_strlen(part));
	if (!ft_strncmp(part, arr[2], ft_strlen(part)) && ++out)
		ft_strcpy(to_add, arr[2] + ft_strlen(part));
	check_builtin2(arr, part, to_add, &out);
	return (out > 1 ? 1 : 0);
}

static void	add_builtins2(char **tab, char **arr, char *part, int *i)
{
	if (!ft_strncmp(part, arr[5], ft_strlen(part)))
		tab[++*i] = ft_strdup(arr[5]);
	if (!ft_strncmp(part, arr[6], ft_strlen(part)))
		tab[++*i] = ft_strdup(arr[6]);
	if (!ft_strncmp(part, arr[7], ft_strlen(part)))
		tab[++*i] = ft_strdup(arr[7]);
}

void		add_builtins(char **tabu, char *part, int *i)
{
	char	*arr[8];

	arr[0] = "setenv";
	arr[1] = "unsetenv";
	arr[2] = "exit";
	arr[3] = "history";
	arr[4] = "export";
	arr[5] = "shellvar";
	arr[6] = "help";
	arr[7] = "unset";
	if (!ft_strncmp(part, arr[0], ft_strlen(part)))
		tabu[++*i] = ft_strdup(arr[0]);
	if (!ft_strncmp(part, arr[1], ft_strlen(part)))
		tabu[++*i] = ft_strdup(arr[1]);
	if (!ft_strncmp(part, arr[2], ft_strlen(part)))
		tabu[++*i] = ft_strdup(arr[2]);
	if (!ft_strncmp(part, arr[3], ft_strlen(part)))
		tabu[++*i] = ft_strdup(arr[3]);
	if (!ft_strncmp(part, arr[4], ft_strlen(part)))
		tabu[++*i] = ft_strdup(arr[4]);
	add_builtins2(tabu, arr, part, i);
}
