#include "21sh.h"

t_tree *basic_red(t_sh *sh, e_token type, t_tree *left)
{
	t_token *token;

	if (sh->current_token->type == type)
	{
		token = sh->current_token;
		eat(sh, type);
		if (!sh->current_token)
			return (NULL);
		return (create_node(left, token, NULL, create_node(NULL, text_rules(sh), NULL, NULL)));
	}
	return (NULL);
}

t_tree *adv_red_forward(t_sh *sh, e_token type, t_tree *left)
{
	t_token *token;
	t_tree *new_node;

	ft_putendl("adv_red_forward");
	new_node = NULL;
	if (sh->current_token->type == type)
	{
		token = sh->current_token;
		eat(sh, type);
		if (!sh->current_token)
			return (NULL);
		if (sh->current_token->type == WORD)
			new_node = create_node(left, token, NULL, create_node(NULL, text_rules(sh), NULL, NULL));
		else if (sh->current_token->type == FD)
		{
			new_node = create_node(left, token, NULL, NULL);
			ft_putendl("NOUNOUK -> ");
			ft_putnbr(sh->current_token->type);
			ft_putnbr(FD);
			new_node->to_fd = ft_atoi(sh->current_token->value);
			eat(sh, FD);
		}
	}
	return (new_node);
}

t_tree *adv_red_backward(t_sh *sh, e_token type, t_tree *left)
{
	t_token *token;
	t_tree *new_node;

	ft_putendl("adv_red_backward");
	new_node = NULL;
	ft_putnbr(type);
	ft_putnbr(sh->current_token->type);
	ft_putendl(" <---");
	ft_putendl(sh->current_token->value);
	if (sh->current_token->type == type)
	{
		token = sh->current_token;
		eat(sh, type);
		if (!sh->current_token)
			return (NULL);
		if (sh->current_token->type == WORD)
		{
			ft_putendl("<& WORD");
			new_node = create_node(left, token, NULL, create_node(NULL, text_rules(sh), NULL, NULL));
		}
		else if (is_string_op(*sh->current_token->value))
		{
			ft_putendl("<& TEXT");
			new_node = create_node(left, token, NULL, create_node(NULL, text_rules(sh), NULL, NULL));
			ft_putendl("ANOUKKKKKKKKKKKKKKKKKKKKKK <3 je crois que je t'aime");
			//		ft_putendl(sh->current_token->value);
		}
		else if (sh->current_token->type == FD)
		{
			ft_putendl("<& NUM");
			new_node = create_node(left, token, NULL, NULL);
			new_node->from_fd = ft_atoi(sh->current_token->value);
			eat(sh, FD);
		}
	}
	return (new_node);
}

t_tree *redirection_rules(t_sh *sh, t_tree *left)
{
	t_token *token;
	t_tree *new_node;

	ft_putendl("WESHHHHHHHHHHHHHHHHHH");
	if (!sh->current_token)
		return (NULL);
	// '>' WORD
	if ((new_node = basic_red(sh, CHEVF, left)))
		ft_putendl("redirection >");
	// '>&' WORD
	// '>&' FD
	else if ((new_node = adv_red_forward(sh, FRED, left)))
		ft_putendl("redirection >&");
	// '>>' WORD
	else if ((new_node = basic_red(sh, DCHEVF, left)))
		ft_putendl("redirection >>");
	// '<' WORD
	else if ((new_node = basic_red(sh, CHEVB, left)))
		ft_putendl("redirection <");
	// '<&' WORD
	// '<&' FD
	else if ((new_node = adv_red_backward(sh, BRED, left)))
		ft_putendl("redirection <&");
	// '<<' WORD
	else if ((new_node = basic_red(sh, DCHEVB, left)))
		ft_putendl("redirection <<");
	else if (sh->current_token->type == FD)
	{
		token = sh->current_token;
		eat(sh, FD);
		ft_putendl("test");
		if (!sh->current_token)
			return ((void *)-1);
		//return (create_node(NULL, token, NULL, NULL));
		// FD '>' WORD
		if ((new_node = basic_red(sh, CHEVF, left)))
			;
		// FD '>&' WORD
		// FD '>&' NUM
		else if ((new_node = adv_red_forward(sh, FRED, left)))
			new_node->from_fd = ft_atoi(token->value);
		// FD '>>' WORD
		else if ((new_node = basic_red(sh, DCHEVF, left)))
			;
		// FD '<' WORD
		else if ((new_node = basic_red(sh, CHEVB, left)))
			;
		// FD '<&' WORD
		// FD '<&' FD
		else if ((new_node = adv_red_backward(sh, BRED, left)))
			new_node->to_fd = ft_atoi(token->value);
		// FD '<<' WORD
		else if ((new_node = basic_red(sh, DCHEVB, left)))
			;
	}
	return (new_node);
}
