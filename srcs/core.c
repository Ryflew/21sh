#include "21sh.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

static char	browse_tree(t_tree *node, t_sh *shell, t_tree *parent, char r_side)
{
	char	ret;
	//t_fd	fd;

	//FD_IN = shell->FD_IN;
	//FD_OUT = shell->FD_OUT;
	//fd_pipe = shell->fd_pipe;
	node->parent = parent;
	ret = 0;
	if (node->token && ((node->left && node->left->cmds) || \
		(node->right && node->right->cmds)))
	{
		if ((operators(node, shell, fd)) == -1)
			return (-1);
//		shell->FD_IN = FD_IN;
//		shell->FD_OUT = FD_OUT;
	//	shell->fd_pipe = fd_pipe;
	}
	//else if (node->cmds && (!node->parent || node->parent->token->type == SCL))
	else if (node->cmds)
	{
		if (node->parent && node->parent->token->type == PIPE)
			shell->right_side = r_side;
		if ((exec_cmds(node, &(shell->env), shell)) == -1)
			return (-1);
	}
	if (node->left)
	{
		if (node->token->type == PIPE)
		{
			if ((ret = browse_tree(node->left, shell, node, 0)) == -1)
				return (-1);
		}
		else if ((ret = browse_tree(node->left, shell, node, r_side)) == -1)
			return (-1);
	}
	if (node->right	&& node->token && (node->token->type < CHEVB || node->token->type > FRED) && (node->token->type != OR || !ret) && (node->token->type != AND || ret) && (ret = browse_tree(node->right, shell, node, r_side)) == -1)
			return (-1);
	return (ret);
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
	shell->fd_in = NULL;
	shell->fds_out = NULL;
	shell->fd_pipe = -1;
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
