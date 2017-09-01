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
	node->token->type > FRED) && (node->token->type != OR || ret) &&
	(node->token->type != AND || !ret))
	{
		if (node->token->type == SCL)
		{
			if (shell->fds_out)
				ft_clear_list(&shell->fds_out, (&free));
			shell->aggregations = shell->aggregations->next;			
			shell->fds_out = (t_list*)shell->aggregations->data;
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
		if (node->parent && node->parent->token->type == PIPE)
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
		//if (shell->fds_out)
		//	ft_clear_list(&shell->fds_out, (&free));
		del_command_tree(commands_tree);
	}
}

void			go_core(char *command, t_sh *shell)
{
	t_tree	*commands_tree;
	t_list	*begin_lexems;

	add_to_history(shell, command);
	shell->lexer->lexems = NULL;
	shell->lexer->line = command;
	get_lexems(shell);
	begin_lexems = shell->lexer->lexems;
	shell->aggregations = NULL;
	shell->current_token = shell->lexer->lexems->data;
	shell->fd_pipe = -1;
	if ((commands_tree = commands_line_rules(shell, NULL)) == (void*)-1)
	{
		parse_error(shell);
		clear(shell, &begin_lexems, NULL);
		return ;
	}
	shell->fds_out = (t_list*)shell->aggregations->data;	
	ft_putendl("after");
	if (commands_tree)
	{
		check_if_env_var(commands_tree);
		if (find_env(shell->env, "PATH"))
			try_add_hashtab(commands_tree, shell);
		browse_tree(commands_tree, shell, NULL, 1);
	}
	clear(shell, &begin_lexems, commands_tree);
	//TO FO free aggregation
}
