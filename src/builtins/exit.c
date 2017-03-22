/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/17 22:25:11 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/03/21 23:17:30 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "21sh.h"

void		errexit(char *content, char *reason)
{
	ft_putstr_fd(content, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(reason, 2);
	exit(EXIT_FAILURE);
}

void		exit_command(char **av, t_sh *shell)
{
	int		i;

	if (!*++av)
	{
		tcsetattr(0, TCSADRAIN, &(shell->old));
		exit(EXIT_SUCCESS);
	}
	i = 0;
	while (*av + i && ((*(*av + i) >= '0' && *(*av + i) <= '9') || \
			*(*av + i) == '-'))
		++i;
	if (!*(*av + i) && !*(av + 1))
	{
		tcsetattr(0, TCSADRAIN, &(shell->old));
		exit(ft_atoi(*av));
	}
	else if ((**av >= '0' && **av <= '9') && *(*av + i))
		ft_putstr("Badly formed number.");
	else if (*(av + 1) || (*(*av + i) < '0' || *(*av + i) > '9' || \
			*(*av + i) != '-'))
		ft_putstr("Expression Syntax.");
}