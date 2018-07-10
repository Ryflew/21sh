/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operators.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/11 17:46:19 by bdurst            #+#    #+#             */
/*   Updated: 2018/07/07 00:20:45 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

char		manage_dchevb(t_sh *sh, char *cmd, t_tree *node, int *fd_pipe)
{
	char	*heredoc_line;
	char	*prompt;

	if (!node->left)
		set_old_term(get_shell(), "123");
	if (!(prompt = find_env(sh->env, "PS2")))
		prompt = "heredoc> ";
	heredoc_line = read_here_doc(cmd, prompt);
	if (!node->left)
		set_our_term(get_shell());
	if (heredoc_line == (void*)-1)
		return (0);
	if (heredoc_line && !(manage_here_doc(sh, heredoc_line, node, fd_pipe)))
		return (0);
	if (node->left)
	{
		(node->to_fd != -1) ? dup2(fd_pipe[0], node->to_fd) : \
			dup2(fd_pipe[0], 0);
		close(fd_pipe[0]);
	}
	return (1);
}

void		manage_chevb(t_fd fd)
{
	if (fd.to != -1)
	{
		dup2(fd.to, 0);
		close(fd.to);
	}
	else
		dup2(fd.file, 0);
	close(fd.file);
}

static char	free_and_return(t_fd *fd)
{
	free(fd);
	return (-1);
}

static char	manage_chev(t_tree *node, t_sh *shell)
{
	t_fd	*fd;

	if (node->TYPE == CHEVF || node->TYPE == DCHEVF || node->TYPE == FRED)
	{
		manage_cmds(node->right, shell, 0);
		if (!(fd = (t_fd*)malloc(sizeof(t_fd))))
			ft_exiterror("Malloc failed", 1);
		if ((fd->file = open_file(node)) == -1)
			return (free_and_return(fd));
		if (node->TYPE != FRED)
			fd->from = (node->from_fd == -1) ? 1 : node->from_fd;
		else
			fd->from = node->from_fd;
		fd->to = (node->to_fd) ? node->to_fd : -1;
		fd->type = node->TYPE;
		ft_node_push_front(&shell->fds_out, fd);
	}
	return (0);
}

char		operators(t_tree *node, t_sh *shell)
{
	if (node->TYPE == PIPE)
	{
		if (shell->fd_pipe == -1)
			shell->fd_pipe = 0;
	}
	else if (node->TYPE == CHEVB)
	{
		manage_cmds(node->right, shell, 0);
		if ((shell->fd_in.file = open_file(node)) == -1)
			return (-1);
		shell->fd_in.to = node->to_fd;
		shell->fd_in.from = -1;
		shell->fd_in.type = node->TYPE;
	}
	else
		return (manage_chev(node, shell));
	return (0);
}
