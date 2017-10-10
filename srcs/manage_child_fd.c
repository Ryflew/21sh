/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_child_fd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/10 20:28:10 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/10/10 20:28:11 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"
#include <fcntl.h>

static void	pipe_subshell(t_sh *shell)
{
	close(shell->pipe_ss[0]);
	dup2(shell->pipe_ss[1], 1);
	close(shell->pipe_ss[1]);
}

static void	close_dup_fd(t_fd *fd)
{
	if (fd->from == -1 && fd->to == -1)
	{
		dup2(fd->file, 1);
		dup2(fd->file, 2);
	}
}

static void	check_fd_is_good(t_tree *node, int left, int right)
{
	if (fcntl(left, F_GETFD) == -1)
	{
		ft_fputstr("21sh: ", 2);
		ft_fputnbr(left, 2);
		ft_fputendl(": Bad file descriptor", 2);
		ft_strdelpp(&node->cmds);
		node->cmds = NULL;
	}
	else
	{
		dup2(left, right);
		close(left);
	}
}

static void	manage_aggregations(t_tree *node)
{
	t_fd	*fd;
	t_list	*tmp;

	tmp = node->aggregations;
	while (tmp)
	{
		fd = tmp->data;
		if (fd->from != -1 && fd->to != -1)
			(fd->type == FRED) ? check_fd_is_good(node, fd->to, fd->from) \
				: check_fd_is_good(node, fd->from, fd->to);
		else if (fd->to == -2)
			(fd->from != -1) ? close(fd->from) : close(1);
		else if (fd->from == -2)
			(fd->to != -1) ? close(fd->to) : close(0);
		tmp = tmp->next;
	}
}

void		manage_child_fd(t_sh *shell, t_tree *node, int *pipe)
{
	t_fd	*fd;
	t_list	*tmp;

	if (!node->parent || node->parent->TYPE != DCHEVB)
		close(pipe[0]);
	child(node, shell, pipe);
	tmp = shell->fds_out;
	while (tmp && node->parent && (shell->fd_pipe == -1 \
		|| shell->right_side))
	{
		fd = tmp->data;
		if ((fd->type != FRED || fd->from != -1))
			dup2(fd->file, fd->from);
		else
			close_dup_fd(fd);
		close(fd->file);
		tmp = tmp->next;
	}
	manage_aggregations(node);
	if (shell->ssbqt)
		pipe_subshell(shell);
}
