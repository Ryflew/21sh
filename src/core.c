/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/18 23:06:58 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/02/28 23:22:56 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

void	go_core(char *command, t_env **env, t_sh *shell)
{
	t_tree		*all_command;
	int			i;

	i = -1;
	all_command = get_command(command);
	free(command);
	while (all_command && all_command[++i])
	{
		// if (!go_builtins(all_command[i], env))
			// go_path(all_command[i], *env);
		// else
		// {
			ft_putstr(tmp[0]);
			ft_putendl(": Command not found.");
		// }
	}
	if (all_command)
		free_tree(&all_command);
}
