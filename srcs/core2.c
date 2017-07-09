#include "21sh.h"

char		exec_cmds(t_tree *node, t_env **env, t_sh *shell)
{
	char	ret;


 	if ((ret = is_writable_builtins(node->cmds[0])) != 1)
	{
						ft_putendl("run buill");				
		run_builtins(node, env, shell);
	}
	else if (!(ret = is_builtins(node->cmds)))
	{
						ft_putendl("go bul");				
		ret = go_builtins(node->cmds, env, shell);
	}
	else
	{
						ft_putendl("run bin");				
		ret = run_binary(node, *env, shell);
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
