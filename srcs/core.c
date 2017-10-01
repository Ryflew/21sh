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

<<<<<<< HEAD
void			manage_tree(t_sh *sh, t_tree *commands_tree)
{
	if (find_env(sh->env, "PATH"))
		try_add_hashtab(commands_tree, sh);
	if (!is_term_env(commands_tree))
=======
char	*get_excla(t_sh *shell, char *start, char *line, int *value)
{
	char	*new;
	int		i;

	i = history_excla(line + 1, shell);
	if (shell->toaddstr)
	{
		*value = (line - start) + ft_strlen(shell->toaddstr);
		start = ft_strsub(start, 0, line - start);
		new = ft_strstrjoin(start, shell->toaddstr, line + 1 + i);
		free(start);
		ft_strdel(&(shell->toaddstr));
		return (new);
	}
	return (NULL);
}

char		check_hist_excla2(t_sh *shell, char *line, int value)
{
	t_list  *lexems;
	t_token *token;
	char	*tmp;
	char	*start;

	start = line;
	lexems = shell->lexer->lexems;
	while (lexems)
	{
		token = (t_token*)lexems->data;
		while (ft_isblank(*line) || *line == '\\' || *line == '"' || *line == '\''|| *line == '`')
			++line;
		if (TYPE == HIST)
		{
			tmp = start;
			start = get_excla(shell, start, line, &value);
			if (shell->lexer->lexems == lexems)
				shell->lexer->lexems = lexems->next;
			ft_pop_node(&lexems, (void*)&clear_lexems);
			if (!start)
			{
				start = tmp;
				++line;
				continue ;
			}
			if (lexems)
				NEXT(lexems);
			ft_strdel(&tmp);
			line = start + value;
		}
		else
		{
			line += ft_strlen(VAL);
			NEXT(lexems);
		}
	}
	if (value > 0)
	{
		shell->toaddstr = start;
		return (0);
	}
	free(start);
	return (1);
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
>>>>>>> d2a1c5acedf3c4fa1a1116705bbdd2f818174bda
	{
		if (shell->fds_out)
			ft_clear_list(&shell->fds_out, (&free));
		del_command_tree(commands_tree);
	}
}

static void	init_shell(t_sh *shell)
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

	add_to_history(shell, command);
	get_lexems(shell);
	glob(shell);
	begin_lexems = shell->lexer->lexems;
	init_shell(shell);
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
