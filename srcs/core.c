#include "tosh.h"

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

void		clear_lexems(t_token *token)
{
	free(VAL);
	free(token);
}

static void	clear(t_sh *shell, t_list **begin, t_tree *commands_tree)
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

void	init_shell_before_parser(t_sh *shell)
{
	if (shell->lexer->lexems)
		shell->current_token = shell->lexer->lexems->data;
	else
		shell->current_token = NULL;
	shell->fd_pipe = -1;
	shell->fds_out = NULL;
}

void		go_core(char *command, t_sh *shell)
{
	t_tree	*commands_tree;
	t_list	*begin_lexems;

	shell->lexer->line = shell->total_command;	
	shell->lexer->her = 0;	
	get_lexems(shell);
	add_to_history(shell, command);
	begin_lexems = shell->lexer->lexems;
	init_shell_before_parser(shell);
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
