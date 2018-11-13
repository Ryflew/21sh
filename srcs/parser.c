/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/11 17:39:15 by bdurst            #+#    #+#             */
/*   Updated: 2018/11/13 14:29:59 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

static char		check_basic_red(t_sh *sh)
{
	t_token	*next_token;

	next_token = NULL;
	if (sh->lexer->lexems && sh->lexer->lexems->next)
		next_token = sh->lexer->lexems->next->data;
	if (sh->current_token && (sh->current_token->type == CHEVF || \
		sh->current_token->type == DCHEVF || sh->current_token->type == CHEVB \
		|| sh->current_token->type == DCHEVB || (sh->current_token->type == FD \
		&& next_token && (next_token->type == DCHEVF || \
		next_token->type == DCHEVF || next_token->type == CHEVB || \
		next_token->type == DCHEVB))))
		return (1);
	return (0);
}

static t_tree	*cmd_with_op_rules(t_sh *sh)
{
	t_tree	*left;
	t_tree	*tmp;
	char	basic_red;

	left = NULL;
	basic_red = 1;
	if (!sh->current_token || (!(left = cmd_rules(sh)) && \
	!(basic_red = check_basic_red(sh))) || left == (void*)-1)
	{
		if (!basic_red)
			left = (void*)-1;
		return (left);
	}
	while ((left || !(basic_red = check_basic_red(sh))) && left != (void*)-1)
	{
		if ((tmp = redirection_rules(sh, left)) && tmp != (void*)-1)
			left = tmp;
		if (!tmp)
			return (left);
		else if (tmp == (void*)-1)
			return (tmp);
	}
	if (!basic_red)
		left = (void*)-1;
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
	while (sh->current_token && sh->current_token->type == PIPE)
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

t_tree			*commands_line_rules(t_sh *sh, t_list **begin_lexems)
{
	t_tree	*left;
	t_tree	*right;
	t_token	*token;

	replace_bqt_subshell(sh);
	sh->current_token = (sh->lexer->lexems) ? sh->lexer->lexems->data : NULL;
	if (begin_lexems)
		*begin_lexems = sh->lexer->lexems;
	while (sh->current_token && sh->current_token->type == SCL)
		eat(sh, SCL);
	if (!(left = condition_operators_rules(sh)) || left == (void*)-1)
		return (left);
	while (sh->current_token && sh->current_token->type == SCL)
	{
		token = sh->current_token;
		while (sh->current_token && sh->current_token->type == SCL)
			eat(sh, SCL);
		if (!sh->current_token)
			break ;
		if ((right = condition_operators_rules(sh)) && right != (void*)-1)
			left = create_node(left, token, NULL, right);
		else
			return (ret_parse_error(left));
	}
	return (left);
}
