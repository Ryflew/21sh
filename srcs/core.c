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
		if (shell->fds_out)
			ft_clear_list(&shell->fds_out, (&free));
		del_command_tree(commands_tree);
	}
}

void	check_if_env_var(t_tree *tree)
{
	int 	i;
	t_env	*cpy;
	char	*ptr;
	char	*tmp;

	if (tree->left)
		return (check_if_env_var(tree->left));
	if (tree->right)
		return (check_if_env_var(tree->right));
	i = -1;
	while (tree->cmds && tree->cmds[++i])
	{
		if (!(ptr = ft_strchr(tree->cmds[i], '$')) && \
			!find_env(cpy, tree->cmds[i]))
			continue ;
		cpy = get_shell()->env;
		while (cpy)
		{
			if (!ft_strcmp(cpy->var_name, ptr + 1))
			{
				tmp = ft_strsub(tree->cmds[i], 0, ptr - tree->cmds[i]);
				free(tree->cmds[i]);
				tree->cmds[i] = ft_strjoin(tmp, cpy->var_content);
				free(tmp);
				break ;
			}
			NEXT(cpy);
		}
	}
}

void			go_core(char *command, t_sh *shell)
{
	t_tree	*commands_tree;
	t_list	*begin;

	add_to_history(shell, command);
	shell->lexer->lexems = NULL;
	shell->lexer->line = command;
	get_lexems(shell);
	begin = shell->lexer->lexems;
	shell->current_token = shell->lexer->lexems->data;
	shell->fds_out = NULL;
	shell->fd_pipe = -1;
	if ((commands_tree = commands_line_rules(shell)) == (void*)-1)
	{
		parse_error(shell);
		clear(shell, &begin, NULL);
		return ;
	}
	if (commands_tree)
	{
		check_if_env_var(commands_tree);		
		if (find_env(shell->env, "PATH"))
			try_add_hashtab(commands_tree, shell);
		browse_tree(commands_tree, shell, NULL, 1);
	}
	clear(shell, &begin, commands_tree);	
}
