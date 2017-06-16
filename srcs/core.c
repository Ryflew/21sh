#include "21sh.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

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

char		exec_cmds(t_tree *node, t_env **env, t_sh *shell)
{
	char	ret;

	if (!node->cmds || (ret = go_builtins(node->cmds, env, shell)) == 1)
	{
		if ((ret = get_path(node, *env, shell, 1)))
			ret = -1;
	}
	return (ret);
}

char		exec_cmds_with_op(t_tree *node, t_env **env, t_sh *shell)
{
	char	ret;

	ret = 0;
	if (node->cmds && (ret = is_writable_builtins(node->cmds[0])))
	{
		if ((ret = get_path(node, *env, shell, 1)))
			ret = -1;
	}
	else
		run_builtins(node, env, shell);
	return (ret);
}

static char	browse_tree(t_tree *node, t_sh *shell, t_tree *parent, char r_side)
{
	node->parent = parent;
	if (node->left)
	{
		if ((browse_tree(node->left, shell, node, 0)) == -1)
			return (-1);
	}
	if (node->right)
	{
		if ((browse_tree(node->right, shell, node, r_side)) == -1)
			return (-1);
	}
	if (node->token && ((node->left && node->left->cmds) || \
		(node->right && node->right->cmds)))
		return (operators(node, &(shell->env), shell, r_side));
	if (node->cmds && (!node->parent || node->parent->token->type == SCL))
		return (exec_cmds(node, &(shell->env), shell));
	return (0);
}

static char	check_cmd(char *cmd, t_sh *shell, t_tree *node)
{
	char	ret;

	if (!cmd || !cmd[0])
		return (-1);
	if (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd") ||
		!ft_strcmp(cmd, "exit") || !ft_strcmp(cmd, "setenv") ||
		!ft_strcmp(cmd, "env") || !ft_strcmp(cmd, "unsetenv") ||
		!ft_strcmp(cmd, "hash"))
		return (0);
	if ((ret = get_path(node, shell->env, shell, 0)) == 1)
	{
		ft_putstr(cmd);
		ft_putendl(": Command not found.");
		return (-1);
	}
	else if (ret == 2)
		return (-1);
	return (0);
}

static char	cmd_validity(t_tree *node, t_sh *shell)
{
	if (node->left)
	{
		if (cmd_validity(node->left, shell) == -1)
			return (-1);
	}
	if (node->right)
	{
		if (node->token->type != CHEVB && node->token->type != DCHEVB &&
		node->token->type != CHEVF && node->token->type != DCHEVF &&
		node->token->type != FRED && cmd_validity(node->right, shell) == -1)
			return (-1);
	}
	if (node->cmds)
		return (check_cmd(node->cmds[0], shell, node));
	return (0);
}

void		clear_lexems(t_token *token)
{
	free(token->value);
	free(token);
}

void		go_core(char *command, t_sh *shell)
{
	t_tree	*commands_tree;

	add_to_history(shell, command);
	shell->lexer->line = command;
	get_lexems(shell);
	shell->current_token = shell->lexer->lexems->data;
	if ((commands_tree = commands_line_rules(shell)) == (void*)-1)
	{
		parse_error(shell);
		ft_clear_list(&shell->lexer->lexems, (void*)&clear_lexems);
		return ;
	}
	if (!cmd_validity(commands_tree, shell))
		browse_tree(commands_tree, shell, NULL, 1);
	ft_clear_list(&shell->lexer->lexems, (void*)&clear_lexems);
	if (shell->save_env)
	{
		del_all_env(&(shell->env));
		shell->env = shell->save_env;
		shell->save_env = NULL;
	}
}
