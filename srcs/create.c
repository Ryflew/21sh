/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/10 20:24:05 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/07/01 22:20:39 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

t_tree	*create_node(t_tree *left, t_token *token, t_list *tokens, t_tree *rig)
{
	t_tree *new_node;

	if (!(new_node = (t_tree *)malloc(sizeof(t_tree))))
		ft_exiterror("Malloc failure", 1);
	new_node->left = left;
	new_node->right = rig;
	new_node->token = token;
	new_node->cmds = NULL;
	new_node->cmd_tokens = tokens;
	new_node->to_fd = -1;
	new_node->from_fd = -1;
	new_node->tmp_env = NULL;
	new_node->aggregations = NULL;
	return (new_node);
}

t_token	*new_token(t_lexer *lexer, enum e_token token_type, char *value,
					char blank)
{
	t_token *token;

	if (!(token = (t_token *)malloc(sizeof(t_token))))
		ft_exiterror("Malloc failed", 1);
	if (lexer)
		lexer->line += ft_strlen(value);
	TYPE = token_type;
	if (value)
		value = ft_strdup(value);
	VAL = value;
	token->blank = blank;
	return (token);
}
