#include "21sh.h"

char		parse_error(t_sh *sh)
{
	ft_putstr("parse error near '");
	if (sh->current_token)
		ft_putstr(sh->current_token->value);
	else
		ft_putstr("\\n");
	ft_putendl("'");
	return (-1);
}

static int	is_delimiter_op(e_token token)
{
	if (token == LPAR || token == RPAR || token == LBKT || token == RBKT || \
		token == LBRC || token == RBRC)
		return (1);
	return (0);
}

static char	aggregation_rules(t_sh *sh, int *fd)
{
	if (!sh->current_token)
		return (0);
	if (sh->current_token->type == FD && sh->lexer->lexems->next && \
		((t_token*)sh->lexer->lexems->next->data)->type == FRED && \
		sh->lexer->lexems->next->next && \
		((t_token*)sh->lexer->lexems->next->next->data)->type == FD)
	{
		fd[0] = ft_atoi(sh->current_token->value);
		eat(sh, FD);
		eat(sh, FRED);
		fd[1] = ft_atoi(sh->current_token->value);
		eat(sh, FD);
		return (1);
	}
	else if (sh->current_token->type == FRED && sh->lexer->lexems->next && \
		((t_token*)sh->lexer->lexems->next->data)->type == FD)
	{
		fd[0] = 1;
		eat(sh, FRED);
		fd[1] = ft_atoi(sh->current_token->value);
		eat(sh, FD);
		return (1);
	}
	else if (sh->current_token->type == FD && sh->lexer->lexems->next && \
		((t_token*)sh->lexer->lexems->next->data)->type == FRED)
	{
		fd[0] = ft_atoi(sh->current_token->value);
		eat(sh, FRED);
		fd[1] = -1;
		eat(sh, FD);
		return (1);
	}
	return (0);
}

static void	cmd_without_delimiter_rules(t_sh *sh, t_tree **new_node)
{
	int		fd[2];
	t_list	*cmd_tokens;
	t_token	*token;

	//ft_putendl("step7");
	cmd_tokens = NULL;
	fd[0] = -1;
	fd[1] = -1;
	while ((token = text_rules(sh)) || (token != (void*)-1 && \
		aggregation_rules(sh, fd)))
	{
		//ft_putendl("step8");
		//ft_putendl(delimiter_token->value);
		if (token)
			ft_node_push_back(&cmd_tokens, token->value);
		//	exit(1);
	}
	if (token == (void*)-1)
		*new_node = (void*)-1;
	if (cmd_tokens)
	{
		*new_node = create_node(NULL, NULL, cmd_tokens, NULL);
		if (fd[0] != -1)
		{
			(*new_node)->from_fd = fd[0];
			(*new_node)->to_fd = fd[1];
		}
	}
}

t_tree		*cmd_rules(t_sh *sh)
{
	t_token	*delimiter_token;
	t_tree	*new_node;

	if (!sh->current_token)
		return (NULL);
	new_node = NULL;
	//ft_putendl("step5");
	if (is_delimiter_op(sh->current_token->type))
	{
		//ft_putendl("step6");
		delimiter_token = sh->current_token;
		eat(sh, sh->current_token->type);
		//ft_putendl(sh->current_token->value);
		new_node = commands_line_rules(sh);
		eat(sh, delimiter_token->type + 1);
	}
	else
		cmd_without_delimiter_rules(sh, &new_node);
	return (new_node);
}
