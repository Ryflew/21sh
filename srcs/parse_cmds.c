#include "tosh.h"

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

void		*ret_parse_error(t_tree *node)
{
	del_command_tree(node);
	return ((void*)-1);
}

static void	cmd_without_delimiter_rules(t_sh *sh, t_tree **new_node)
{
	t_fd	*fd;
	t_list	*cmd_tokens;
	t_token	*token;
	t_list	*aggregations;

	cmd_tokens = NULL;
	fd = NULL;
	aggregations = NULL;
	while ((token = text_rules(sh)) || (token != (void*)-1 && \
		(fd = aggregation_rules(sh))))
	{
		if (fd)
			ft_node_push_back(&aggregations, fd);
		if (token)
			ft_node_push_back(&cmd_tokens, ft_strdup(token->value));
	}
	if (token == (void*)-1)
		*new_node = (void*)-1;
	if (cmd_tokens)
	{
		*new_node = create_node(NULL, NULL, cmd_tokens, NULL);
		(*new_node)->aggregations = aggregations;
	}
}

t_tree		*cmd_rules(t_sh *sh)
{
	t_tree	*new_node;

	if (!sh->current_token)
		return (NULL);
	new_node = NULL;
	if (sh->current_token->type == LPAR)
	{
		eat(sh, LPAR);
		new_node = commands_line_rules(sh);
		eat(sh, RPAR);
	}
	else
		cmd_without_delimiter_rules(sh, &new_node);
	return (new_node);
}
