#include "tosh.h"

char			is_basic_red(t_sh *sh)
{
	t_token	*next_token;

	next_token = NULL;
	if (sh->lexer->lexems && sh->lexer->lexems->next)
		next_token = sh->lexer->lexems->next->data;
	if (sh->current_token && (sh->current_token->type == CHEVF || sh->current_token->type == DCHEVF\
	|| sh->current_token->type == CHEVB || sh->current_token->type \
	== DCHEVB || (sh->current_token->type == FD && next_token &&
		(next_token->type == DCHEVF || next_token->type == DCHEVF \
		||  next_token->type == CHEVB || next_token->type == DCHEVB))))
		return (1);
	return (0);
}

static t_tree	*cmd_with_op_rules(t_sh *sh)
{
	t_tree	*left;
	t_tree	*tmp;

	left = NULL;
	if (!sh->current_token || (!(left = cmd_rules(sh)) && !is_basic_red(sh)) || left == (void*)-1)
		return (left);
	while ((left || is_basic_red(sh)) && left != (void*)-1)
	{
		if ((tmp = redirection_rules(sh, left)) && tmp != (void*)-1)
			left = tmp;
		if (!tmp)
			return (left);
		else if (tmp == (void*)-1)
			return (tmp);
	}
	return (left);
}

static t_tree	*pipe_rules(t_sh *sh)
{
	t_tree	*left;
	t_token	*token;
	t_tree	*right;

	left = NULL;
	if (!sh->current_token || !(left = cmd_with_op_rules(sh)) \
		|| left == (void*)-1)
		return (left);
	while (sh->current_token &&  sh->current_token->type == PIPE)
	{
		if (!left->right && left->token && left->TYPE != SCL)
			return (ret_parse_error(left));
		token = sh->current_token;
		eat(sh, PIPE);
		if ((right = cmd_with_op_rules(sh)) && right != (void*)-1)
			left = create_node(left, token, NULL, right);
		else
			return (ret_parse_error(left));
	}
	return (left);
}

static t_tree	*condition_operators_rules(t_sh *sh)
{
	t_tree	*left;
	t_tree	*right;
	t_token	*token;

	if (!(left = pipe_rules(sh)) || left == (void*)-1)
		return (left);
	while (sh->current_token && (sh->current_token->type == OR
	|| sh->current_token->type == AND))
	{
		token = sh->current_token;
		if (TYPE == OR)
			eat(sh, OR);
		else if (TYPE == AND)
			eat(sh, AND);
		if ((right = pipe_rules(sh)) != (void*)-1 && right)
			left = create_node(left, token, NULL, right);
		else
			return (ret_parse_error(left));
	}
	return (left);
}

static t_tree	*parse_semicolons(t_sh *sh, t_tree *left)
{
	t_tree	*right;
	t_token	*token;

	while (sh->current_token &&  sh->current_token->type == SCL)
	{
		token = sh->current_token;
		while (sh->current_token &&  sh->current_token->type == SCL)
			eat(sh, SCL);
		if ((right = condition_operators_rules(sh))
			&& right != (void*)-1)
			left = create_node(left, token, NULL, right);
		else
			return (ret_parse_error(left));
	}
	return (left);
}

t_tree			*commands_line_rules(t_sh *sh)
{
	t_tree	*left;

	while (sh->current_token &&  sh->current_token->type == SCL)
		eat(sh, SCL);
	if (!(left = condition_operators_rules(sh)) || left == (void*)-1)
		return (left);
	return (parse_semicolons(sh, left));
}
