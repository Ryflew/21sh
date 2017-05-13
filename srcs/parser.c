#include "21sh.h"

void eat(t_sh *sh, e_token token)
{
	if (sh->current_token->type == token)
	{
		sh->lexer->lexems = sh->lexer->lexems->next;
		if (sh->lexer->lexems)
			sh->current_token = sh->lexer->lexems->data;
		else
		sh->current_token = NULL;
		if (sh->current_token == NULL)
			;//ft_putendl("NULLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL");
	}
	else
	{
		ft_putstr("parse error near '");
		ft_putstr(sh->current_token->value);
		ft_putendl("'");
		exit(-1);
	}
	//	syntax_error();
}

static t_tree *pipe_rules(t_sh *sh, t_tree *left)
{
	t_token *token;
	t_tree *new_node;

	//ft_putendl("step9-1-1");
	new_node = NULL;
	while (sh->current_token && sh->current_token->type == PIPE)
	{
		//ft_putendl("step9-1-2");
		token = sh->current_token;
		//ft_putendl(sh->current_token->value);
		eat(sh, PIPE);
		//ft_putendl(token->value);
		if ((new_node = create_node(left, token, NULL, cmd_rules(sh))))
			left = new_node;
		//else
			//ft_putendl("YESSSSSSSS");
	}
	//ft_putendl("step9-1-3");
	//		ft_putendl(sh->current_token->value);
	return (new_node);
}

static t_tree *cmd_with_op_rules(t_sh *sh)
{
	t_tree *left;
	t_tree *tmp;

	//ft_putendl("step4");
	left = cmd_rules(sh);
	//ft_putendl(((t_token *)left->tokens)->value);
	//ft_putendl("step9");
	while (left)
	{
		//ft_putendl("step9-0");
		//		ft_putendl(sh->current_token->value);
		if ((tmp = pipe_rules(sh, left)))
			left = tmp;
		else if ((tmp = redirection_rules(sh, left)))
		{
			//ft_putendl("step9-2");
			left = tmp;
		}
		if (!tmp)
		{
			//ft_putendl("step9-3");
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

static t_tree *condition_operators_rules(t_sh *sh)
{
	t_tree *left;
	t_tree *right;
	t_token *token;

	left = cmd_with_op_rules(sh);
	//ft_putendl("step12");
	while (sh->current_token && (sh->current_token->type == OR || sh->current_token->type == AND))
	{
		//ft_putendl("step13");
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

t_tree *commands_line_rules(t_sh *sh)
{
	t_tree *left;
	t_tree *right;
	t_token *token;

	//ft_putendl("step1");
	//ft_putendl(sh->current_token->value);
	if (sh->current_token && sh->current_token->type == SCL) // TODO BUG TO FIXE
	{
		//ft_putendl("step2");
		eat(sh, SCL);
	}
	//ft_putendl("step3");
	left = condition_operators_rules(sh);
	//ft_putendl("step10");
	while (sh->current_token && sh->current_token->type == SCL)
	{
		//ft_putendl("step11");
		token = sh->current_token;
		eat(sh, SCL);
		if ((right = condition_operators_rules(sh)))
			left = create_node(left, token, NULL, right);
	}
	return (left);
}
