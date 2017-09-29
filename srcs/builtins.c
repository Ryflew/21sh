#include "tosh.h"

char	go_builtins2(char **cmd, t_sh *shell)
{
	if (!ft_strcmp(cmd[0], "history"))
		history_cmd(cmd, shell);
	else if (!ft_strcmp(cmd[0], "hash"))
		hash_tab(cmd, shell);
	else if (!ft_strcmp(cmd[0], "read"))
		read_builtin(shell, cmd);
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
		echo_builtin(cmd, *env);
		shell->return_value = 0;
		return (0);
	}
	shell->return_value = 0;
	if (!ft_strcmp(cmd[0], "cd"))
		cd(cmd, *env, shell);
	else if (!ft_strcmp(cmd[0], "setenv"))
		set_env(cmd, env);
	else if (!ft_strcmp(cmd[0], "unsetenv"))
		unset_env(cmd, env, NULL);
	else if (!ft_strcmp(cmd[0], "unset"))
		unset_env(cmd, env, &(shell->export));
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

char	is_writable_builtins(char *cmd)
{
	if (!ft_strcmp(cmd, "echo"))
		return (0);
	else if (!ft_strcmp(cmd, "env"))
		return (0);
	else if (!ft_strcmp(cmd, "history")) // ? sur que c'est dans is writable ?
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
	else if (!ft_strcmp(cmd[0], "unset"))
		return (0);
	else if (!ft_strcmp(cmd[0], "export"))
		return (0);
	else if (!ft_strcmp(cmd[0], "exit"))
		return (0);
	else if (!ft_strcmp(cmd[0], "hash"))
		return (0);
	else if (!ft_strcmp(cmd[0], "true"))
		return (0);
	else if (!ft_strcmp(cmd[0], "false"))
		return (0);
	else if (!ft_strcmp(cmd[0], "help"))
		return (0);
	else if (!ft_strcmp(cmd[0], "read"))
		return (0);
	else
		return (is_writable_builtins(cmd[0]));
}
