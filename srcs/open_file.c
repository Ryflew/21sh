/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/11 17:46:33 by bdurst            #+#    #+#             */
/*   Updated: 2017/10/11 19:40:43 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"
#include <fcntl.h>

static int	open_chevb(t_tree *node)
{
	int		fd_file;

	if ((fd_file = open(node->right->cmds[0], O_RDONLY)) == -1)
		errexit(node->right->cmds[0], "File open failure");
	return (fd_file);
}

static int	open_dchevf(t_tree *node)
{
	int		fd_file;

	if ((fd_file = open(node->right->cmds[0], O_WRONLY | O_APPEND | \
		O_CREAT, 0644)) == -1)
		errexit(node->right->cmds[0], "File open failure");
	return (fd_file);
}

static int	open_chevf(t_tree *node)
{
	int		fd_file;

	if ((fd_file = open(node->right->cmds[0], O_WRONLY | O_CREAT | \
		O_TRUNC, 0644)) == -1)
		errexit(node->right->cmds[0], "File open failure");
	return (fd_file);
}

int			open_file(t_tree *node)
{
	int	ret;

	ret = 0;
	if (node->token && node->TYPE == CHEVB)
		return (open_chevb(node));
	else if (node->token && (node->TYPE == CHEVF || \
		node->TYPE == FRED))
		return (open_chevf(node));
	else if (node->token && node->TYPE == DCHEVF)
		return (open_dchevf(node));
	else
		return (ret);
}
