/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binary2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/10 20:23:06 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/07/11 01:35:10 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include <signal.h>
#include "tosh.h"

static void	run_with_pipe(t_sh *shell, int *fd, t_tree *node)
{
	if (shell->fd_pipe != 0 && node->parent
		&& node->parent->TYPE != DCHEVB && node->parent->TYPE != CHEVB)
	{
		dup2(shell->fd_pipe, 0);
		close(shell->fd_pipe);
	}
	if (fd[1] != 1 && !shell->right_side)
	{
		dup2(fd[1], 1);
		close(fd[1]);
	}
}

void		child(t_tree *node, t_sh *shell, int *fd, int *heredoc_pipe)
{
	t_list	*tmp;
	char	*cmd;

	if (node->parent && node->parent->TYPE == DCHEVB)
	{
		if (node->parent->right)
		{
			tmp = node->parent->right->cmd_tokens;
			while (tmp)
			{
				cmd = ((t_token*)tmp->data)->value;
				tmp = tmp->next;
			}
			manage_dchevb(shell, cmd, node->parent, \
				heredoc_pipe);
		}
	}
	else if (node->parent && node->parent->TYPE == CHEVB)
		manage_chevb(shell->fd_in);
	if (shell->fd_pipe != -1)
		run_with_pipe(shell, fd, node);
}

char		stop_binary(int sig)
{
	int	ret;

	if (sig == SIGTERM)
	{
		if (g_father != -1)
		{
			kill(g_father, sig);
			waitpid(g_father, &ret, WNOHANG);
			ft_putchar('\n');
		}
		exit(EXIT_SUCCESS);
	}
	if (!g_father)
		exit(EXIT_SUCCESS);
	if (g_father != -1 && sig == SIGINT)
	{
		kill(g_father, sig);
		g_father = -1;
		ft_putchar('\n');
		return (1);
	}
	return (0);
}

int			get_fd(t_sh *shell, int *fd)
{
	int	ret;

	ret = 0;
	if (shell->fd_pipe != -1)
		if ((ret = pipe(fd)) == -1)
			errexit("21sh", "pipe failure !\n");
	return (ret);
}
