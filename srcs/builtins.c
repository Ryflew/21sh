/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/10 20:23:15 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/11/15 17:23:39 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

char	go_builtins2(char **cmd, t_sh *shell)
{
	if (!ft_strcmp(cmd[0], "history"))
		history_cmd(cmd, shell);
	else if (!ft_strcmp(cmd[0], "hash"))
		hash_tab(cmd, shell);
	else if (!ft_strcmp(cmd[0], "read"))
		read_builtin(shell, cmd);
	else if (!ft_strcmp(cmd[0], "shellvar"))
		shellvar(shell->shell_var);
	else if (!ft_strcmp(cmd[0], "exit"))
		exit_command(cmd, shell);
	else if (!ft_strcmp(cmd[0], "help"))
		help_command(cmd);
	else if (!ft_strcmp(cmd[0], "false"))
	{
		shell->return_value = 1;
		return (1);
	}
	else
		return (1);
	return (0);
}

char	go_builtins(char **cmd, t_env **env, t_sh *shell)
{
	if (!ft_strcmp(cmd[0], "echo"))
	{
		echo_builtin(cmd);
		return (shell->return_value);
	}
	if (!ft_strcmp(cmd[0], "cd"))
		cd(cmd, *env, shell);
	else if (!ft_strcmp(cmd[0], "setenv"))
		set_env(cmd, env);
	else if (!ft_strcmp(cmd[0], "unsetenv"))
		unset_env(cmd, env);
	else if (!ft_strcmp(cmd[0], "unset"))
		manage_unset(shell, cmd);
	else if (!ft_strcmp(cmd[0], "export"))
		export(cmd, &(shell->export));
	else if (!ft_strcmp(cmd[0], "env"))
		env_command(cmd, *env);
	else if (!ft_strcmp(cmd[0], "true"))
		return (0);
	else
		return (go_builtins2(cmd, shell));
	return (0);
}

char	is_writable_builtins(char **cmd)
{
	if (!ft_strcmp(cmd[0], "echo"))
		return (0);
	else if (!ft_strcmp(cmd[0], "env") && !cmd[1])
		return (0);
	else if (!ft_strcmp(cmd[0], "history") && !cmd[1])
		return (0);
	else if (!ft_strcmp(cmd[0], "export") && !cmd[1])
		return (0);
	else if (!ft_strcmp(cmd[0], "hash") && !cmd[1])
		return (0);
	else if (!ft_strcmp(cmd[0], "help"))
		return (0);
	return (1);
}

char	is_builtins2(char **cmd)
{
	if (!ft_strcmp(cmd[0], "shellvar"))
		return (0);
	else
		return (is_writable_builtins(cmd));
}

char	is_builtins(char **cmd)
{
	if (!ft_strcmp(cmd[0], "cd"))
		return (0);
	else if (!ft_strcmp(cmd[0], "setenv"))
		return (0);
	else if (!ft_strcmp(cmd[0], "unsetenv"))
		return (0);
	else if (!ft_strcmp(cmd[0], "unset"))
		return (0);
	else if (!ft_strcmp(cmd[0], "exit"))
		return (0);
	else if (!ft_strcmp(cmd[0], "true"))
		return (0);
	else if (!ft_strcmp(cmd[0], "false"))
		return (0);
	else if (!ft_strcmp(cmd[0], "read"))
		return (0);
	else if (!ft_strcmp(cmd[0], "history"))
		return (0);
	else if (!ft_strcmp(cmd[0], "export"))
		return (0);
	else if (!ft_strcmp(cmd[0], "hash"))
		return (0);
	else
		return (is_builtins2(cmd));
}
