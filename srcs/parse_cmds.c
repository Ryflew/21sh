#include "tosh.h"

char			parse_error(t_sh *sh)
{
	ft_putstr("parse error near '");
	if (sh->current_token)
		ft_putstr(sh->current_token->value);
	else
		ft_putstr("\\n");
	ft_putendl("'");
	sh->return_value = 1;
	return (-1);
}

void			*ret_parse_error(t_tree *node)
{
	del_command_tree(node);
	return ((void*)-1);
}

static t_token	*ret_get_tokens(t_fd *fd, t_token *token, t_list **cmd_tokens)
{
	if (fd == (void*)-1)
	{
		ft_clear_list(cmd_tokens, (void*)&clear_lexems);		
		return ((void*)-1);
	}
	return (token);
}

static t_token	*get_tokens_cmd(t_sh *sh, t_list **aggregations, \
					t_token **ss_empty, t_list **cmd_tokens)
{
	t_token	*token;
	t_fd	*fd;

	fd = NULL;
	while (((token = text_rules(sh)) && token != (void*)-1) \
	|| (token != (void*)-1 && (fd = aggregation_rules(sh)) && fd != (void*)-1))
	{
		if (fd)
			ft_node_push_back(aggregations, fd);
		fd = NULL;
		if (token)
		{
			if (TYPE != NONE)
				ft_node_push_back(cmd_tokens, new_token(NULL, TYPE, VAL));
			else
			{
				if (*ss_empty)
					clear_lexems(*ss_empty);
				*ss_empty = token;
			}
		}
	}
	return (ret_get_tokens(fd, token, cmd_tokens));
}

t_tree			*cmd_rules(t_sh *sh)
{
	t_list	*cmd_tokens;
	t_list	*aggreg;
	t_token	*ss_empty;
	t_tree	*new_node;
	char	**tmp_env;

	aggreg = NULL;
	ss_empty = 0;
	cmd_tokens = NULL;
	new_node = NULL;
	tmp_env = parse_env_cmds(sh);
	if ((get_tokens_cmd(sh, &aggreg, &ss_empty, &cmd_tokens)) == (void*)-1)
		new_node = (void*)-1;
	else if (cmd_tokens)
	{
		if (ss_empty)
			clear_lexems(ss_empty);
		new_node = create_node(NULL, NULL, cmd_tokens, NULL);
		new_node->aggregations = aggreg;
	}
	else if (ss_empty || tmp_env)
		new_node = create_node(NULL, ss_empty, NULL, NULL);
	if (tmp_env)
		new_node->tmp_env = tmp_env;
	return (new_node);
}
