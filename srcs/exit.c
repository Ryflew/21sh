/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/10 20:24:53 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/09/29 18:06:56 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "tosh.h"

void		errexit(char *content, char *reason)
{
	get_shell()->return_value = 1;
	get_shell()->have_write_error = 1;
	ft_fputstr(content, 2);
	ft_fputstr(": ", 2);
	ft_fputendl(reason, 2);
}

void		exit_command(char **av, t_sh *shell)
{
	int		i;

	if (!*++av)
	{
		save_history(shell);
		tcsetattr(0, TCSADRAIN, &(shell->old));
		exit(shell->return_value);
	}
	i = 0;
	while (*av + i && ((*(*av + i) >= '0' && *(*av + i) <= '9') || \
			*(*av + i) == '-'))
		++i;
	if (!*(*av + i) && !*(av + 1))
	{
		tcsetattr(0, TCSADRAIN, &(shell->old));
		save_history(shell);
		exit(ft_atoi(*av));
	}
	else if ((**av >= '0' && **av <= '9') && *(*av + i))
		errexit("exit", "Badly formed number.");
	else if (*(av + 1) || (*(*av + i) < '0' || *(*av + i) > '9' || \
			*(*av + i) != '-'))
		errexit("exit", "Expression Syntax.");
}
