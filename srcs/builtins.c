#include "21sh.h"

char 	go_builtins(char **cmd, t_env **env, t_sh *shell)
{
	if (!ft_strcmp(cmd[0], "echo"))
		echo_builtin(cmd, *env);
	else if (!ft_strcmp(cmd[0], "cd"))
		cd(cmd, *env, shell);
	else if (!ft_strcmp(cmd[0], "setenv"))
		set_env(cmd, env);
	else if (!ft_strcmp(cmd[0], "unsetenv"))
		unset_env(cmd, env);
	else if (!ft_strcmp(cmd[0], "env"))
		env_command(cmd, *env);
	else if (!ft_strcmp(cmd[0], "exit"))
		exit_command(cmd, shell);
	else if (!ft_strcmp(cmd[0], "hash"))
		hash_tab(cmd, shell);
	else if (!ft_strcmp(cmd[0], "true"))
		return (0);
	else if (!ft_strcmp(cmd[0], "false"))
		return (1);
	else
		return (1);
	return (0);
}

char	is_writable_builtins(char *cmd)
{
	if (!ft_strcmp(cmd, "echo"))
		return (0);
	else if (!ft_strcmp(cmd, "env"))
		return (0);
	return (1);
}

char	is_builtins(char **cmd)
{
	if (!ft_strcmp(cmd[0], "cd"))
		return (0);
	else if (!ft_strcmp(cmd[0], "setenv"))
		return (0);
	else if (!ft_strcmp(cmd[0], "unsetenv"))
		return (0);
	else if (!ft_strcmp(cmd[0], "exit"))
		return (0);
	else if (!ft_strcmp(cmd[0], "hash"))
		return (0);
	else if (!ft_strcmp(cmd[0], "true"))
		return (0);
	else if (!ft_strcmp(cmd[0], "false"))
		return (0);
	else
		return (is_writable_builtins(cmd[0]));
}