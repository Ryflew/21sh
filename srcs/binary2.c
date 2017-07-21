#include <sys/types.h>
#include <signal.h>
#include "21sh.h"

void	child(t_tree *node, t_sh *shell, int *fd)
{
	if (node->parent && node->parent->token->type == DCHEVB)
		manage_dchevb(node, node->parent->right->cmds[0], 1);
	else if (node->parent && node->parent->token->type == CHEVB)
		manage_chevb(shell->fd_in);
	if (shell->fd_pipe != -1)
		run_with_pipe(shell, fd);
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

void	run_with_pipe(t_sh *shell, int *fd)
{
	if (shell->fd_pipe != 0)
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

int		get_fd(t_sh *shell, int *fd)
{
	int	ret;

	ret = 0;
	if (shell->fd_pipe != -1)
		if ((ret = pipe(fd)) == -1)
			ft_putstr("pipe failure !\n");
	return (ret);
}
