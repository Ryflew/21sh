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
	(node->token->type != AND || !ret))
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

static char			is_term_env(t_tree *tree)
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

void	check_if_home_tilde(t_tree *tree, char *home)
{
	int		i;
	char	*ptr;
	char	*tmp;
	char	*tmp2;

	if (tree->left)
		return (check_if_env_var(tree->left));
	if (tree->right)
		return (check_if_env_var(tree->right));
	i = -1;
	while (tree->cmds && tree->cmds[++i])
	{
		if ((ptr = ft_strstr(tree->cmds[i], "~/")))
		{
			tmp = ft_strsub(tree->cmds[i], 0, ptr - tree->cmds[i]);
			tmp2 = ft_strstrjoin(tmp, home, ptr + 1);
			free(tmp);
			free(tree->cmds[i]);
			tree->cmds[i] = tmp2;
		}
	}
}

char	*get_excla(t_sh *shell, char *start, char *line, int *value)
{
	char	*new;
	int		i;

	i = history_excla(line + 1, shell);
	if (shell->toaddstr)
	{
		*value = (line - start) + ft_strlen(shell->toaddstr);
		start = ft_strsub(start, 0, line - start);
		new = ft_strstrjoin(start, shell->toaddstr, line + i);
		free(start);
		ft_strdel(&(shell->toaddstr));
		return (new);
	}
	return (NULL);
}

char		check_hist_exla2(t_sh *shell, char *line, int value)
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
		if (token->type == HIST)
		{
			tmp = start;
			start = get_excla(shell, start, line, &value);
			if (!start)
			{
				start = tmp;
				NEXT(lexems);
				continue ;
			}
			ft_strdel(&tmp);
			line = start + value;
		}
		else
		{
			line += ft_strlen(token->value);
			NEXT(lexems);
		}
	}
	if (value > 0)
	{
		shell->toaddstr = start;
		return (0);
	}
	return (1);
}

void			go_core(char *command, t_sh *shell)
{
	t_tree	*commands_tree;
	t_list	*begin_lexems;

	get_lexems(shell);
	if (!check_hist_exla2(shell, ft_strdup(command), 0))
		return ;
	add_to_history(shell, command);
	glob(shell);
	begin_lexems = shell->lexer->lexems;
	shell->current_token = shell->lexer->lexems->data;
	shell->fd_pipe = -1;
	shell->fds_out = NULL;
	if ((commands_tree = commands_line_rules(shell)) == (void*)-1)
	{
		parse_error(shell);
		clear(shell, &begin_lexems, NULL);
		return ;
	}
	if (commands_tree)
	{
		check_if_env_var(commands_tree);
		if (find_env(shell->env, "HOME"))
			check_if_home_tilde(commands_tree, find_env(shell->env, "HOME"));
		if (find_env(shell->env, "PATH"))
			try_add_hashtab(commands_tree, shell);
		if (!is_term_env(commands_tree))
		{
			treat_history_cmd(commands_tree);
			browse_tree(commands_tree, shell, NULL, 1);
		}
	}
	clear(shell, &begin_lexems, commands_tree);
}
