/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/18 23:06:58 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/03/28 00:07:43 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "21sh.h"

static char	go_builtins(t_tree *tree, t_env **env, t_sh *shell)
{
	(void)env;
	(void)shell;
	if (!ft_strcmp(tree->cmd[0], "echo"))
		;
		// echo(av, *env);
	else if (!ft_strcmp(tree->cmd[0], "cd"))
		;
		// cd(av, *env, path);
	else if (!ft_strcmp(tree->cmd[0], "setenv"))
		;
		// set_env(av, env);
	else if (!ft_strcmp(tree->cmd[0], "unsetenv"))
		;
		// unset_env(av, env);
	else if (!ft_strcmp(tree->cmd[0], "env"))
		;
		// env_command(av, *env);
	else if (!ft_strcmp(tree->cmd[0], "exit"))
		exit_command(tree->cmd, shell);
	// else
		// return (0);
	return (1);
}

static void	add_to_history(t_sh *shell, char *command)
{
	int		i;
	char	*tmp;
	
	if (!ft_strcont(command, "\n"))
		add_line(shell, command);
	else
	{
		i = -1;
		while (command[++i] && command[i] != '\n')
			;
		tmp = ft_strsub(command, 0, i);
		add_line(shell, tmp);
		free(tmp);
	}
	shell->history_pos = -1;
}

void	go_core(char *command, t_env **env, t_sh *shell)
{
	t_tree		**all_command;
	int			i;

	(void)env;
	add_to_history(shell, command);
	if (!(all_command = get_command(command)))
		return ;
	i = -1;
	while (all_command[++i])
	{
		if (go_builtins(all_command[i], env, shell)/* || \
			go_path(all_command[i], *env)*/)
			;
		else
		{
			ft_putstr(all_command[i]->cmd[0]);
			ft_putendl(": Command not found.");
		}
	}
	free_tree(all_command);
}
