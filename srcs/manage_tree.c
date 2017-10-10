/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_tree.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/10 20:28:18 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/10/10 20:28:18 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

static void	reset_pipe(t_sh *shell)
{
	shell->fds_out = NULL;
	shell->fd_pipe = -1;
	shell->right_side = 1;
}

static char	manage_children(t_tree *node, t_sh *shell, char rig, char ret)
{
	if (node->left)
	{
		if (node->TYPE == PIPE &&
			(ret = browse_tree(node->left, shell, node, 0)) == -1)
			return (-1);
		else if (node->TYPE != PIPE &&
				(ret = browse_tree(node->left, shell, node, rig)) == -1)
			return (-1);
	}
	if (node->right && node->token && (node->TYPE < CHEVB || node->TYPE > BRED)
		&& (node->TYPE != OR || (ret && (node->left->token && \
		node->left->TYPE == NONE))) && (node->TYPE != AND || !ret || \
		(node->left->token && node->left->TYPE == NONE)))
	{
		if (node->TYPE == SCL)
		{
			if (shell->fds_out)
				ft_clear_list(&shell->fds_out, (&free));
			reset_pipe(shell);
		}
		if ((ret = browse_tree(node->right, shell, node, rig)) == -1)
			return (-1);
	}
	return (ret);
}

char		browse_tree(t_tree *node, t_sh *shell, t_tree *parent, char rig)
{
	char	ret;

	node->parent = parent;
	ret = 0;
	if (node->token && ((node->left && node->left->cmd_tokens) || \
		(node->right && node->right->cmd_tokens)))
	{
		if ((operators(node, shell)) == -1)
			return (-1);
	}
	else if (node->cmd_tokens)
	{
		if (node->parent && shell->fd_pipe != -1)
			shell->right_side = rig;
		if ((ret = manage_cmds(node, shell)) == -1)
			return (-1);
	}
	return (manage_children(node, shell, rig, ret));
}
