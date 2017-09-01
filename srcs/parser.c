#include "tosh.h"

static t_tree	*pipe_rules(t_sh *sh, t_tree *left, t_list **aggregation)
{
	t_token	*token;
	t_tree	*new_node;

	new_node = NULL;
	while (sh->current_token && sh->current_token->type == PIPE)
	{
		if (!left->right && left->token && left->token->type != SCL)
			return (ret_parse_error(left));
		token = sh->current_token;
		eat(sh, PIPE);
		if ((new_node = create_node(left, token, NULL, cmd_rules(sh, aggregation))))
			left = new_node;
	}
	return (new_node);
}

static t_tree	*cmd_with_op_rules(t_sh *sh, t_list **aggregation)
{
	t_tree	*left;
	t_tree	*tmp;

	left = NULL;
	if (!sh->current_token || (!(left = cmd_rules(sh, aggregation)) && \
		sh->current_token->type != CHEVF && sh->current_token->type != DCHEVF\
		&& sh->current_token->type != CHEVB && sh->current_token->type \
		!= DCHEVB) || (!left && sh->lexer->lexems && sh->lexer->lexems->prev &&
		((t_token*)(sh->lexer->lexems->prev->data))->type != SCL) \
		|| left == (void*)-1)
		return (left);
	while ((left || (sh->current_token->type == CHEVF || \
		sh->current_token->type == DCHEVF || sh->current_token->type == CHEVB \
		|| sh->current_token->type == DCHEVB)) && left != (void*)-1)
	{
		if ((tmp = pipe_rules(sh, left, aggregation)) && tmp != (void*)-1)
			left = tmp;
		else if ((tmp = redirection_rules(sh, left)) && tmp != (void*)-1)
			left = tmp;
		if (!tmp)
			return (left);
		else if (tmp == (void*)-1)
			return (tmp);
	}
	return (left);
}

static t_tree	*condition_operators_rules(t_sh *sh, t_list **aggregation)
{
	t_tree	*left;
	t_tree	*right;
	t_token	*token;

	if (!(left = cmd_with_op_rules(sh, aggregation)) || left == (void*)-1)
		return (left);
	while (sh->current_token && (sh->current_token->type == OR
	|| sh->current_token->type == AND))
	{
		token = sh->current_token;
		if (token->type == OR)
			eat(sh, OR);
		else if (token->type == AND)
			eat(sh, AND);
		if ((right = cmd_with_op_rules(sh, aggregation)) != (void*)-1 && right)
			left = create_node(left, token, NULL, right);
		else
			return (ret_parse_error(left));
	}
	return (left);
}

static t_tree	*parse_semicolons(t_sh *sh, t_tree *left, t_list **aggregation)
{
	t_tree	*right;
	char	**tmp_env;
	t_token	*token;

	while (sh->current_token && sh->current_token->type == SCL)
	{
		token = sh->current_token;
		while (sh->current_token && sh->current_token->type == SCL)
			eat(sh, SCL);
		tmp_env = parse_env_cmds(sh);
		if ((right = condition_operators_rules(sh, aggregation)) && right != (void*)-1)
		{
			ft_node_push_back(&sh->aggregations, aggregation);
			*aggregation = NULL;
			right->tmp_env = tmp_env;
			left = create_node(left, token, NULL, right);
		}
		else
			return (ret_parse_error(left));
	}
	return (left);
}

t_tree			*commands_line_rules(t_sh *sh, t_list **aggreg)
{
	t_tree	*left;
	char	**tmp_env;
	t_list	*aggregation;

	while (sh->current_token && sh->current_token->type == SCL)
		eat(sh, SCL);
	tmp_env = parse_env_cmds(sh);
	if ((!aggreg && !(left = condition_operators_rules(sh, &aggregation))) || (aggreg && !(left = condition_operators_rules(sh, aggreg))) || left == (void*)-1)
	{
		if (tmp_env)
			free(tmp_env);
		return (left);
	}
	if (!aggreg)
	{
		ft_node_push_back(&sh->aggregations, aggregation);
		aggregation = NULL;
	}
	left->tmp_env = tmp_env;
	if (aggreg)
		return (parse_semicolons(sh, left, aggreg));
	else
		return (parse_semicolons(sh, left, &aggregation));
}
