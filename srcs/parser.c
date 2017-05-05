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
	{
		sh->current_token = get_next_token(sh->lexer);
		if (sh->current_token == NULL)
			ft_putendl("NULLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL");
	}
	else
					ft_putendl("SYNTAX ERROR");
	//	syntax_error();
}

t_token	*str_rules(t_sh *sh)
{
	t_token	*old_token;
	t_token	*token;

	if (!sh->current_token)
		return (NULL);
	token = NULL;
	ft_putendl("step7-1-1");
//	ft_putendl(sh->current_token->value);
	if (is_string_op(*sh->current_token->value))
	{
		ft_putendl("step7-1-2");
		old_token = sh->current_token;
		eat(sh, sh->current_token->type);
		ft_putendl("step7-1-3");
	//ft_putendl(sh->current_token->value);
	//ft_putnbr(sh->current_token->type);
	//ft_putnbr(TEXT);
		token = sh->current_token;
		eat(sh, TEXT);
		ft_putendl("step7-1-4");
	//ft_putnbr(sh->current_token->type);
	//ft_putnbr(old_token->type);
		eat(sh, old_token->type);
	}
	ft_putendl("step7-1-5");
	return (token);
}

t_token	*text_rules(t_sh *sh)
{
	t_token *token;

	if (!sh->current_token)
		return (NULL);
	token = sh->current_token;
	ft_putendl("step7-1");
	ft_putendl(token->value);
	if (sh->current_token->type == WORD)
	{
	ft_putendl("step7-2");
		eat(sh, WORD);
	}
	else if (sh->current_token->type == NUM)
		eat(sh, NUM);
	else
		token = str_rules(sh);
	ft_putendl("step7-3 ret token");
	if (token)
					ft_putendl(token->value);
	return (token);
}

t_tree	*cmd_rules(t_sh *sh)
{
	t_list	*cmd_tokens;
	t_token	*delimiter_token;
	t_tree	*new_node;

	if (!sh->current_token)
		return (NULL);
	cmd_tokens = NULL;
	new_node = NULL;
	ft_putendl("step5");
	if (is_delimiter_op(sh->current_token->type))
	{
	ft_putendl("step6");
		delimiter_token = sh->current_token;
		eat(sh, sh->current_token->type);
	ft_putendl(sh->current_token->value);
		new_node = commands_line_rules(sh);
		eat(sh, delimiter_token->type);
	}
	else
	{
	ft_putendl("step7");
		while ((delimiter_token = text_rules(sh)))
		{
	ft_putendl("step8");
	ft_putendl(delimiter_token->value);
			ft_node_push_back(&cmd_tokens, delimiter_token->value);
//	exit(1);
		}
		new_node = create_node(NULL, NULL, cmd_tokens, NULL);
	}
	return (new_node);
}

t_tree	*pipe_rules(t_sh *sh, t_tree *left)
{
	t_token	*token;
	t_tree	*new_node;

	ft_putendl("step9-1-1");
	new_node = NULL;
	while (sh->current_token && sh->current_token->type == PIPE)
	{
	ft_putendl("step9-1-2");
		token = sh->current_token;
		ft_putendl(sh->current_token->value);
		eat(sh, PIPE);
		//ft_putendl(token->value);
		if ((new_node = create_node(left, token, NULL, cmd_rules(sh))))
			left = new_node;
		else
					ft_putendl("YESSSSSSSS");
	}
	ft_putendl("step9-1-3");
//		ft_putendl(sh->current_token->value);
	return (new_node);
}

t_tree	*cmd_with_op_rules(t_sh *sh)
{
	t_tree	*left;
	t_tree	*tmp;
	
	ft_putendl("step4");
	left = cmd_rules(sh);
	ft_putendl(((t_token*)left->tokens)->value);
	ft_putendl("step9");
	while (left)
	{
		ft_putendl("step9-0");
//		ft_putendl(sh->current_token->value);
		if ((tmp = pipe_rules(sh, left)))
			left = tmp;
		else if ((tmp = redirection_rules(sh, left)))
		{
		ft_putendl("step9-2");
			left = tmp;
		}
		if (!tmp)
		{
		ft_putendl("step9-3");
			return (left);
		}
	}
	return (left);
}

/*void		init(t_sh *sh)
{
	sh->lexer->string_operator = 0;
	sh->lexer->red = 0;
	sh->current_token = get_next_token(sh->lexer);
}*/

t_tree	*condition_operators_rules(t_sh *sh)
{
	t_tree	*left;
	t_tree	*right;
	t_token	*token;

	left = cmd_with_op_rules(sh);
	ft_putendl("step12");
	while (sh->current_token && (sh->current_token->type == OR || sh->current_token->type == AND))
	{
	ft_putendl("step13");
		token = sh->current_token;
		if (token->type == OR)
						eat(sh, OR);
		else if (token->type == AND)
						eat(sh, AND);
		if ((right = cmd_with_op_rules(sh)))
			left = create_node(left, token, NULL, right);
	}
	return (left);
}

t_tree	*commands_line_rules(t_sh *sh)
{
	t_tree	*left;
	t_tree	*right;
	t_token	*token;

	ft_putendl("step1");;
	ft_putendl("step1-1");
	//ft_putendl(sh->current_token->value);
	if (sh->current_token && sh->current_token->type == SCL)
	{
		ft_putendl("step2");
		eat(sh, SCL);
	}
	ft_putendl("step3");
	left = condition_operators_rules(sh);
	ft_putendl("step10");
	while (sh->current_token && sh->current_token->type == SCL)
	{
	ft_putendl("step11");
		token = sh->current_token;
		eat(sh, SCL);
		if ((right = condition_operators_rules(sh)))
			left = create_node(left, token, NULL, right);
	}
	return (left);
}
