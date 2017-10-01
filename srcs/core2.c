#include "tosh.h"

char	exec_cmds(t_tree *node, t_env **env, t_sh *shell)
{
	char	ret;

	if ((ret = is_writable_builtins(node->cmds[0])) != 1)
		run_builtins(node, env, shell);
	else if (!(ret = is_builtins(node->cmds)))
		ret = go_builtins(node->cmds, env, shell);
	else
		ret = run_binary(node, *env, shell);
	return (ret);
}

void	add_to_history(t_sh *shell, char *command)
{
	int		i;
	char	*tmp;

	if (!command || !*command)
		return ;
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
