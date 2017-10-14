/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_parser.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdurst <bdurst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/11 17:39:07 by bdurst            #+#    #+#             */
/*   Updated: 2017/10/11 17:44:10 by bdurst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

t_tree			*basic_red(t_sh *sh, enum e_token type, t_tree *left)
{
	t_token	*token;
	t_tree	*right;

	if (left && !left->right && left->token && left->TYPE != SCL && \
		left->TYPE != NONE)
		return (ret_parse_error(left));
	if (sh->current_token->type == type)
	{
		token = sh->current_token;
		eat(sh, type);
		if (!sh->current_token)
			return (ret_parse_error(left));
		if (!(right = cmd_rules(sh)) || right == (void*)-1)
			return (ret_parse_error(left));
		if (!left)
			left = create_node(NULL, NULL, ft_create_node(new_token(NULL, WORD, "")), NULL);
		return (create_node(left, token, NULL, right));
	}
	return (NULL);
}

t_tree			*adv_red_forward(t_sh *sh, enum e_token type, t_tree *left)
{
	t_token	*token_type;
	t_tree	*right;

	if (left && !left->right && left->token && left->TYPE != SCL && \
		left->TYPE != NONE)
		return (ret_parse_error(left));
	if (sh->current_token->type == type)
	{
		token_type = sh->current_token;
		eat(sh, type);
		if (!sh->current_token)
			return (ret_parse_error(left));
		if (!(right = cmd_rules(sh)) || right == (void*)-1)
			return (ret_parse_error(left));
		return (create_node(left, token_type, NULL, right));
	}
	return (NULL);
}

static t_tree	*redirection_with_fd(t_sh *sh, t_tree *left, t_tree *new_node)
{
	t_token	*token;

	if (left && !left->right && left->token && left->TYPE != SCL && \
		left->TYPE != NONE)
		return (ret_parse_error(left));
	token = sh->current_token;
	eat(sh, FD);
	if (!sh->current_token)
		return (ret_parse_error(left));
	if ((new_node = basic_red(sh, CHEVF, left)) && new_node != (void*)-1)
		new_node->from_fd = ft_atoi(VAL);
	else if (new_node != (void*)-1 && (new_node = adv_red_forward(sh, FRED, \
		left)) && new_node != (void*)-1)
		new_node->from_fd = ft_atoi(VAL);
	else if (new_node != (void*)-1 && (new_node = basic_red(sh, DCHEVF, \
		left)) && new_node != (void*)-1)
		new_node->from_fd = ft_atoi(VAL);
	else if (new_node != (void*)-1 && (new_node = basic_red(sh, CHEVB, \
		left)) && new_node != (void*)-1)
		new_node->to_fd = ft_atoi(VAL);
	else if (new_node != (void*)-1 && (new_node = basic_red(sh, DCHEVB, \
		left)) && new_node != (void*)-1)
		new_node->to_fd = ft_atoi(VAL);
	return (new_node);
}

t_tree			*redirection_rules(t_sh *sh, t_tree *left)
{
	t_tree	*new_node;

	if (!sh->current_token)
		return (NULL);
	if ((new_node = basic_red(sh, CHEVF, left)))
		;
	else if ((new_node = adv_red_forward(sh, FRED, left)))
		;
	else if ((new_node = basic_red(sh, DCHEVF, left)))
		;
	else if ((new_node = basic_red(sh, CHEVB, left)))
		;
	else if ((new_node = basic_red(sh, DCHEVB, left)))
		;
	else if (sh->current_token->type == FD)
		new_node = redirection_with_fd(sh, left, NULL);
	return (new_node);
}
