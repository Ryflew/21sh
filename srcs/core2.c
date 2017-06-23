#include "21sh.h"

char		exec_cmds(t_tree *node, t_env **env, t_sh *shell)
{
	char	ret;


	if ((ret = is_writable_builtins(node->cmds[0])) != 1)
	{
		run_builtins(node, env, shell);
	}
	else if ((ret = go_builtins(node->cmds, env, shell)) == 1)
	{
		if ((ret = get_path(node, *env, shell, 1)))
			ret = -1;
	}
	return (ret);
}

void		add_to_history(t_sh *shell, char *command)
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
