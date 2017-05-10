#include <stdlib.h>
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

static void add_to_history(t_sh *shell, char *command)
{
	int i;
	char *tmp;

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

char	exec_cmds(char **cmd, t_env **env, t_sh *shell, char pipe)
{
	char	ret;

	if ((ret = go_builtins(cmd, env, shell)) == 1)
	{
	 	if ((ret = get_path(cmd, *env, pipe, 1)))
		{
			ft_putstr(cmd[0]);
			ft_putendl(": Command not found.");
		}
	}
	return (ret);
}


void browse_tree(t_tree *node, t_env **env, t_sh *shell, t_tree *parent, char right_side)
{
	node->parent = parent;
	if (node->left)
		browse_tree(node->left, env, shell, node, 0);
	if (node->right)
		browse_tree(node->right, env, shell, node, right_side);
	if (node->token && (node->left->cmds || node->right->cmds))
		operators(node, env, shell, right_side);
	if (node->cmds && (!node->parent || (node->parent->token->type != PIPE && node->parent->token->type != AND && node->parent->token->type != OR)))
		exec_cmds(node->cmds, env, shell, 0);
}

void go_core(char *command, t_env **env, t_sh *shell)
{
	t_tree *commands_tree;

	add_to_history(shell, command);
	shell->lexer->line = command;
	shell->current_token = get_next_token(shell->lexer);
	if (!(commands_tree = commands_line_rules(shell)))
		return;
	browse_tree(commands_tree, env, shell, NULL, 1);
}
