#include "21sh.h"

char go_builtins(char **cmd, t_env **env, t_sh *shell)
{
	(void)env;
	(void)shell;
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
	else if (!ft_strcmp(cmd[0], "true"))
		return (0);
	else if (!ft_strcmp(cmd[0], "false"))
		return (1);
	else
		return (1);
	return (0);
}

char is_builtins(char **cmd)
{
	if (!ft_strcmp(cmd[0], "echo"))
		;
	else if (!ft_strcmp(cmd[0], "cd"))
		;
	else if (!ft_strcmp(cmd[0], "setenv"))
		;
	else if (!ft_strcmp(cmd[0], "unsetenv"))
		;
	else if (!ft_strcmp(cmd[0], "env"))
		;
	else if (!ft_strcmp(cmd[0], "exit"))
		;
	else if (!ft_strcmp(cmd[0], "true"))
		;
	else if (!ft_strcmp(cmd[0], "false"))
		;
	else
		return (1);
	return (0);
}