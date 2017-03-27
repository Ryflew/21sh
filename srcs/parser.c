#include "21sh.h"

int		is_delimiter_op(e_token token)
{
	if (token == LPAR || token == RPAR || token == LBKT || token == RBKT || token == LBRC || token == RBRC)
					return (1);
	return (0);
}

t_tree	*create_node(t_tree *left, t_token *token, t_list *tokens, t_tree *right)
{
	t_tree	*new_node;

	if (!(new_node = (t_tree*)malloc(sizeof(t_tree))))
		ft_exiterror("Malloc failure", -1);
	new_node->left = left;
	new_node->right = right;
	new_node->token = token;
	new_node->tokens = tokens;
	return (new_node);
}

void	eat(t_sh *sh, e_token token)
{
	if (sh->current_token->type == token)
		sh->current_token = get_next_token(sh->lexer);
	else
					ft_putendl("SYNTAX ERROR");
	//	syntax_error();
}

t_token	*str(t_sh *sh)
{
	t_token	*old_token;
	t_token	*token;

	token = NULL;
	if (is_string_op(*sh->current_token->value))
	{
		old_token = sh->current_token;
		eat(sh, sh->current_token->type);
		token = sh->current_token;
		eat(sh, TEXT);
		eat(sh, old_token->type);
	}
	return (token);
}

t_token	*text(t_sh *sh)
{
	t_token *token;

	token = sh->current_token;
	if (sh->current_token->type == WORD)
		eat(sh, WORD);
	else if (sh->current_token->type == NUM)
		eat(sh, NUM);
	else
		token = str(sh);
	return (token);
}

t_tree	*cmd(t_sh *sh)
{
	t_list	*cmd_tokens;
	t_token	*delimiter_token;
	t_tree	*new_node;

	cmd_tokens = NULL;
	new_node = NULL;
	if (is_delimiter_op(sh->current_token->type))
	{
		delimiter_token = sh->current_token;
		eat(sh, sh->current_token->type);
		new_node = cmds_line(sh);
		eat(sh, delimiter_token->type);
	}
	else
	{
		while ((delimiter_token = text(sh)))
			ft_node_push_back(&cmd_tokens, delimiter_token);
		new_node = create_node(NULL, NULL, cmd_tokens, NULL);
	}
	return (new_node);
}

t_tree	*pipe_rule(t_sh *sh, t_tree *left)
{
	t_token	*token;

	while (sh->current_token->type == PIPE)
	{
		token = sh->current_token;
		eat(sh, PIPE);
		left = create_node(left, token, NULL, cmd(sh));
	}
	return (left);
}

t_tree	*cmd_with_op(t_sh *sh)
{
	t_tree	*left;
	
	left = cmd(sh);
	while ((left && (left = pipe_rule(sh, left))) || (left = redirection(sh, left)))
		left = pipe_rule(sh, left);
	return (left);
}

t_tree	*cmds_line(t_sh *sh)
{
	t_tree	*left;
	t_tree	*right;
	t_token	*token;

	if (sh->current_token->type == SCL)
		eat(sh, SCL);
	left = cmd_with_op(sh);
	while (sh->current_token->type == SCL)
	{
		token = sh->current_token;
		eat(sh, SCL);
		if ((right = cmd_with_op(sh)))
			left = create_node(left, token, NULL, right);
	}
	return (left);
}
