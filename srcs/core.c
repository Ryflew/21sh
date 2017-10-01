#include "tosh.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

static char		manage_children(t_tree *node, t_sh *shell, char rig, char ret)
{
	if (node->left)
	{
		if (node->token->type == PIPE)
		{
			if ((ret = browse_tree(node->left, shell, node, 0)) == -1)
				return (-1);
		}
		else if ((ret = browse_tree(node->left, shell, node, rig)) == -1)
			return (-1);
	}
	if (node->right && node->token && (node->token->type < CHEVB ||
	node->token->type > BRED) && (node->token->type != OR || ret) &&
	(node->token->type != AND || !ret || node->left->token->type == NONE))
	{
		if (node->token->type == SCL)
		{
			if (shell->fds_out)
				ft_clear_list(&shell->fds_out, (&free));
			shell->fds_out = NULL;
			shell->fd_pipe = -1;
			shell->right_side = 1;
		}
		if ((ret = browse_tree(node->right, shell, node, rig)) == -1)
			return (-1);
	}
	return (ret);
}

char			browse_tree(t_tree *node, t_sh *shell, t_tree *parent, char rig)
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
		if (node->parent && shell->fd_pipe != -1)
			shell->right_side = rig;
		if ((ret = exec_cmds(node, &(shell->env), shell)) == -1)
			return (-1);
	}
	return (manage_children(node, shell, rig, ret));
}

void			clear_lexems(t_token *token)
{
	free(token->value);
	free(token);
}

static void		clear(t_sh *shell, t_list **begin, t_tree *commands_tree)
{
	ft_clear_list(begin, (void*)&clear_lexems);
	if (shell->save_env)
	{
		if (shell->env)
			del_all_env(&(shell->env));
		shell->env = shell->save_env;
		shell->save_env = NULL;
	}
	if (commands_tree)
	{
		if (shell->fds_out)
			ft_clear_list(&shell->fds_out, (&free));
		del_command_tree(commands_tree);
	}
}

char			is_term_env(t_tree *tree)
{
	if (!tree->left && !tree->right && tree->cmds && tree->cmds[0] && \
		!tree->cmds[1] && ft_strchr(tree->cmds[0], '='))
	{
		add_new_var(&(tree->cmds), &(get_shell()->export));
		return (1);
	}
	else
		return (0);
}

void			manage_tree(t_sh *sh, t_tree *commands_tree)
{
	if (find_env(sh->env, "PATH"))
		try_add_hashtab(commands_tree, sh);
	if (!is_term_env(commands_tree))
	{
		treat_history_cmd(commands_tree);
		browse_tree(commands_tree, sh, NULL, 1);
	}
}

void			go_core(char *command, t_sh *shell)
{
	t_tree	*commands_tree;
	t_list	*begin_lexems;

	add_to_history(shell, command);
	get_lexems(shell);
	glob(shell);
	begin_lexems = shell->lexer->lexems;
	if (shell->lexer->lexems)
		shell->current_token = shell->lexer->lexems->data;
	else
		shell->current_token = NULL;
	shell->fd_pipe = -1;
	shell->fds_out = NULL;
	if ((commands_tree = commands_line_rules(shell)) == (void*)-1)
	{
		parse_error(shell);
		clear(shell, &begin_lexems, NULL);
		return ;
	}
	if (commands_tree)
		manage_tree(shell, commands_tree);
	clear(shell, &begin_lexems, commands_tree);
}
