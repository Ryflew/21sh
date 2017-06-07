#include "21sh.h"
#include <stdlib.h>

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

char	exec_cmds(t_tree *node, t_env **env, t_sh *shell)
{
	char	ret;

	if (!node->cmds || (ret = go_builtins(node->cmds, env, shell)) == 1)
	{
	 	if ((ret = get_path(node, *env, shell, 1)))
		{
			ft_putstr(node->cmds[0]);
			ft_putendl(": Command not found.");
			ret = -1;
		}
	}
	return (ret);
}


static char browse_tree(t_tree *node, t_env **env, t_sh *shell, t_tree *parent, char right_side)
{
	node->parent = parent;
	if (node->left)
	{
		if ((browse_tree(node->left, env, shell, node, 0)) == -1)
			return (-1);
	}
	if (node->right)
	{
		if ((browse_tree(node->right, env, shell, node, right_side)) == -1)
			return (-1);
	}
	if (node->token && ((node->left && node->left->cmds) || (node->right && node->right->cmds)))
		return (operators(node, env, shell, right_side));
	if (node->cmds && (!node->parent || node->parent->token->type == SCL)) //!= PIPE && node->parent->token->type != AND && node->parent->token->type != OR && node->parent->token->type != CHEVB)))
		return (exec_cmds(node, env, shell));
	return (0);
}

void	clear_lexems(t_token *token)
{
	free(token->value);
	free(token);
}

void go_core(char *command, t_env **env, t_sh *shell)
{
	t_tree *commands_tree;

	add_to_history(shell, command);
	shell->lexer->line = command;
	get_lexems(shell);
	shell->current_token = shell->lexer->lexems->data;
	if ((commands_tree = commands_line_rules(shell)) == (void*)-1)//|| commands_tree == (void*)-1)
	{
		parse_error(shell);
		ft_clear_list(&shell->lexer->lexems, (void*)&clear_lexems);		
		return ;
	}
	browse_tree(commands_tree, env, shell, NULL, 1);
	ft_clear_list(&shell->lexer->lexems, (void*)&clear_lexems);
}
