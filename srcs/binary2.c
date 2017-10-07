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

void	child(t_tree *node, t_sh *shell, int *fd)
{
	if (node->parent && node->parent->TYPE == DCHEVB)
		manage_dchevb(shell, node->parent->right->cmds[0], node->parent, fd);
	else if (node->parent && node->parent->TYPE == CHEVB)
		manage_chevb(shell->fd_in);
	if (shell->fd_pipe != -1)
		run_with_pipe(shell, fd, node);
}

char	stop_binary(int sig)
{
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

int		get_fd(t_sh *shell, int *fd, t_tree *node)
{
	int	ret;

	ret = 0;
	if (shell->fd_pipe != -1 || (shell->ssbqt && !(shell->pipe_ss)[0])
		|| (node && node->TYPE == DCHEVB))
		if ((ret = pipe(fd)) == -1)
			ft_putstr("pipe failure !\n");
	return (ret);
}
