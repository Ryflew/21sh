#include <sys/types.h>
#include <signal.h>
#include "21sh.h"

void	child(t_tree *node, t_sh *shell, int *fd)
{
	if (node->token && node->parent->token->type == DCHEVB)
		manage_dchevb(node, node->cmds[0], shell->fds_out);
	else if (node->parent && node->parent->token->type == CHEVB)
		manage_chevb(shell->fd_in);
	if (shell->fd_pipe != -1)
		run_with_pipe(shell, fd);
}

char		stop_binary(int sig)
{
	pid_t	child;

	child = child_pid();
	if (child == 0)
		exit(EXIT_SUCCESS);
	if (child != -1 && sig == SIGINT)
	{
		kill(child, sig);
		child = -1;
		ft_putchar('\n');
		return (1);
	}
	return (0);
}

void		run_with_pipe(t_sh *shell, int *fd)
{
	if (shell->fd_pipe != 0)
	{
	//	ft_fputendl("pipe 0", 2);
		dup2(shell->fd_pipe, 0);
		close(shell->fd_pipe);
	}
	if (fd[1] != 1 && !shell->right_side)
	{
	//	ft_fputendl("pipe 1", 2);
		dup2(fd[1], 1);
		close(fd[1]);
	}		
}

int			open_file(t_tree *node)
{
	int	ret;

	ret = 0;
	if (node->token && node->token->type == CHEVB)
		return (open_chevb(node));
	else if (node->token && (node->token->type == CHEVF || \
		node->token->type == FRED))
		return (open_chevf(node));
	else if (node->token && node->token->type == DCHEVF)
		return (open_dchevf(node));
	else
		return (ret);
}

int			get_fd(t_sh *shell, int *fd)
{
	int	ret;

	ret = 0;
	if (shell->fd_pipe != -1)
		if ((ret = pipe(fd)) == -1)
			ft_putstr("pipe failure !\n");
	return (ret);
}
