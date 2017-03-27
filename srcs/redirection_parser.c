#include "21sh.h"

t_tree	*basic_red(t_sh *sh, e_token type, t_tree *left)
{
	t_token *token;

	if (sh->current_token->type == type)
	{
		token = sh->current_token;
		eat(sh, type);
		return (create_node(left, token, NULL, create_node(NULL, text(sh), NULL, NULL)));
	}
	return (NULL);
}

t_tree	*adv_red_forward(t_sh *sh, e_token type, t_tree *left)
{
	t_token	*token;
	t_tree	*new_node;

	new_node = NULL;
	if (sh->current_token->type == type)
	{
		token = sh->current_token;
		eat(sh, type);
		if (sh->current_token->type == WORD)
			new_node = create_node(left, token, NULL, create_node(NULL, text(sh), NULL, NULL));
		else if (sh->current_token->type == NUM)
		{
			new_node = create_node(left, token, NULL, NULL);
			new_node->to_fd = ft_atoi(sh->current_token->value);
			eat(sh, NUM);
		}
	}
	return (new_node);
}

t_tree	*adv_red_backward(t_sh *sh, e_token type, t_tree *left)
{
	t_token	*token;
	t_tree	*new_node;

	new_node = NULL;
	if (sh->current_token->type == type)
	{
		token = sh->current_token;
		eat(sh, type);
		if (sh->current_token->type == WORD)
			new_node = create_node(left, token, NULL, create_node(NULL, text(sh), NULL, NULL));
		else if (sh->current_token->type == NUM)
		{
			new_node = create_node(left, token, NULL, NULL);
			new_node->from_fd = ft_atoi(sh->current_token->value);
			eat(sh, NUM);
		}
	}
	return (new_node);
}

t_tree	*redirection(t_sh *sh, t_tree *left)
{
	t_token	*token;
	t_tree	*new_node;

	// '>' WORD
	if ((new_node = basic_red(sh, CHEVF, left)))
		;
	// '>&' WORD
	// '>&' NUM
	else if ((new_node = adv_red_forward(sh, FRED, left)))
		;
	// '>>' WORD
	else if ((new_node = basic_red(sh, DCHEVF, left)))
		;
	// '<' WORD
	else if ((new_node = basic_red(sh, CHEVB, left)))
		;
	// '<&' WORD
	// '<&' NUM
	else if ((new_node = adv_red_backward(sh, BRED, left)))
		;
	// '<<' WORD
	else if ((new_node = basic_red(sh, DCHEVB, left)))
		;
	else if (sh->current_token->type == NUM)
	{
		token = sh->current_token;
		eat(sh, NUM);
		// NUM '>' WORD
		if ((new_node = basic_red(sh, CHEVF, left)))
			;
		// NUM '>&' WORD
		// NUM '>&' NUM
		else if ((new_node = adv_red_forward(sh, FRED, left)))
			new_node->from_fd = ft_atoi(token->value);	
		// NUM '>>' WORD
		else if ((new_node = basic_red(sh, DCHEVF, left)))
			;
		// NUM '<' WORD
		else if ((new_node = basic_red(sh, CHEVB, left)))
			;
		// NUM '<&' WORD
		// NUM '<&' NUM
		else if ((new_node = adv_red_backward(sh, BRED, left)))
			new_node->to_fd = ft_atoi(token->value);	
		// NUM '<<' WORD
		else if ((new_node = basic_red(sh, DCHEVB, left)))
			;
	}
	return (new_node);
}
