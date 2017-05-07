#include <stdlib.h>
#include "21sh.h"

static char go_builtins(char **cmd, t_env **env, t_sh *shell)
{
	(void)env;
	(void)shell;
	if (!ft_strcmp(cmd[0], "echo"))
		;
	// echo(cmd, *env);
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
		return (1);
	else if (!ft_strcmp(cmd[0], "false"))
		return (0);
	else
		return (-1);
	return (1);
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

char	exec_cmds(char **cmd, t_env **env, t_sh *shell)
{
	char	ret;

	if ((ret = go_builtins(cmd, env, shell)) == -1)
	{
	 	if (!(ret = get_path(cmd, *env)))
		{
			ft_putstr(cmd[0]);
			ft_putendl(": Command not found.");
		}
	}
	return (ret);
}


void browse_tree(t_tree *node, t_env **env, t_sh *shell, e_token parent_type)
{
	int fd_in;

	fd_in = -1;
	node->parent_type = parent_type;
	if (node->token)
					ft_putendl(node->token->value);
	if (node->left)
		browse_tree(node->left, env, shell, node->token->type);
	if (node->right)
		browse_tree(node->right, env, shell, node->token->type);
	if (node->token)
		operators(node, &fd_in, env, shell);
	if (node->tokens && node->parent_type != PIPE && node->parent_type != AND && node->parent_type != OR)
		exec_cmds(list_to_tabstr(node->tokens), env, shell);
}

void go_core(char *command, t_env **env, t_sh *shell)
{
	t_tree *commands_tree;

	(void)env;
	add_to_history(shell, command);
	shell->lexer->line = command;
	shell->current_token = get_next_token(shell->lexer);
	if (!(commands_tree = commands_line_rules(shell)))
		return;
	browse_tree(commands_tree, env, shell, NONE);
}
