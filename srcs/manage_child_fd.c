/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_child_fd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/10 20:28:10 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/10/11 17:20:56 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"
#include <fcntl.h>

static void	pipe_subshell(t_sh *sh)
{
	int		*fd;

	fd = (int*)ft_go_end_list(sh->pipe_ss)->data;
	close(fd[0]);
	dup2(fd[1], 1);
	if (sh->lexer->her)
		dup2(fd[1], 2);
	close(fd[1]);
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
		if (fd->to == -7895474)
			(fd->from != -1) ? close(fd->from) : close(1);
		else if (fd->from == -7895474)
			(fd->to != -1) ? close(fd->to) : close(0);
		else if (fd->from != -1 && fd->to != -1)
			(fd->type == FRED) ? check_fd_is_good(node, fd->to, fd->from) \
				: check_fd_is_good(node, fd->from, fd->to);
		tmp = tmp->next;
	}
}

void		manage_child_fd(t_sh *shell, t_tree *node, int *pipe, \
				int *heredoc_pipe)
{
	t_fd	*fd;
	t_list	*tmp;

	close(pipe[0]);
	if (shell->ssbqt == BQT)
		pipe_subshell(shell);
	child(node, shell, pipe, heredoc_pipe);
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
}
