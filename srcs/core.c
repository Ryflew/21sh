#include "21sh.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

static char	manage_children(t_tree *node, t_sh *shell, char r_side, char ret)
{
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
	if (node->right && node->token && (node->token->type < CHEVB ||
	node->token->type > FRED) && (node->token->type != OR || ret) &&
	(node->token->type != AND || !ret))
	{
		if (node->token->type == SCL)
		{
			shell->fds_out = NULL;
			shell->fd_pipe = -1;
			shell->right_side = 1;
		}
		if ((ret = browse_tree(node->right, shell, node, r_side)) == -1)
			return (-1);
	}
	return (ret);
}

char		browse_tree(t_tree *node, t_sh *shell, t_tree *parent, char r_side)
{
	char	ret;

	node->parent = parent;
	ret = 0;
	if (node->token && ((node->left && node->left->cmds) || \
		(node->right && node->right->cmds)))
	{
		if ((operators(node, shell)) == -1)
			return (-1);
	}
	else if (node->cmds)
	{
		if (node->parent && node->parent->token->type == PIPE)
			shell->right_side = r_side;
		if ((ret = exec_cmds(node, &(shell->env), shell)) == -1)
			return (-1);
	}
	return (manage_children(node, shell, r_side, ret));
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
	shell->fds_out = NULL;
	shell->fd_pipe = -1;
	if ((commands_tree = commands_line_rules(shell)) == (void*)-1)
	{
		parse_error(shell);
		ft_clear_list(&shell->lexer->lexems, (void*)&clear_lexems);
		return ;
	}
	ft_clear_list(&shell->lexer->lexems, (void*)&clear_lexems);
	if (commands_tree)
	{
		if (find_env(shell->env, "PATH"))
			try_add_hashtab(commands_tree, shell);
		browse_tree(commands_tree, shell, NULL, 1);
		if (shell->save_env)
		{
			del_all_env(&(shell->env));
			shell->env = shell->save_env;
			shell->save_env = NULL;
		}
		del_command_tree(commands_tree);
	}
}
