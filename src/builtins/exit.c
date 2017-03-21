/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/17 22:25:11 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/03/21 05:47:40 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "21sh.h"

void		errexit(char *content, char *reason)
{
	ft_putstr_fd(content, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(reason, 2);
}

void		exit_command(char **av)
{
	int		i;

	if (!*++av)
		exit(EXIT_SUCCESS);
	i = 0;
	while (*av + i && ((*(*av + i) >= '0' && *(*av + i) <= '9') || \
			*(*av + i) == '-'))
		++i;
	if (!*(*av + i) && !*(av + 1))
		exit(ft_atoi(*av));
	else if ((**av >= '0' && **av <= '9') && *(*av + i))
		errexit("exit", "Badly formed number.");
	else if (*(av + 1) || (*(*av + i) < '0' || *(*av + i) > '9' || \
			*(*av + i) != '-'))
		errexit("exit", "Expression Syntax.");
}