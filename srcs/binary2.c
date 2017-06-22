#include <sys/types.h>
#include <signal.h>
#include "21sh.h"

static void	child2(t_tree *node, char ***cmds)
{
	if (node->from_fd != -1 && node->to_fd != -1)
		dup2(node->from_fd, node->to_fd);
	else if (node->to_fd != -1)
		dup2(1, node->to_fd);
	*cmds = node->cmds;
}

char		**child(t_tree *node, t_sh *shell, int *fd)
{
	char	**cmds;
	char	**envi;

	cmds = NULL;
	if (node->token && node->parent->token->type == DCHEVB)
		cmds = manage_dchevb(node);
	else if (node->token && node->parent->token->type == CHEVB)
	{
		envi = conv_env(shell->env);
		cmds = manage_chevb(node, shell->fd_in, envi);
		if (envi)
			ft_strdelpp(&envi);
	}
	else if (node->token && node->parent->token->type == DCHEVF)
		cmds = manage_dchevf(node, shell->fd_out);
	else if (node->token && node->parent->token->type == CHEVF)
		cmds = manage_chevf(node, shell->fd_out);
	else if (node->token && node->parent->token->type == FRED)
		cmds = manage_fred(node, shell->fd_out);
	else if (shell->fd_in != -1)
		cmds = run_with_pipe(node, shell, fd);
	else
		child2(node, &cmds);
	return (cmds);
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

char		**run_with_pipe(t_tree *node, t_sh *shell, int *fd)
{
	if (node->from_fd != -1 && node->to_fd != -1)
		dup2(fd[1], node->from_fd);
	dup2(shell->fd_in, 0);
	if (!shell->right_side)
	{
		if (shell->fd_out )
			dup2(fd[1], shell->fd_out);
		else
			dup2(fd[1], 1);
	}
	close(fd[0]);
	return (node->cmds);
}

int			open_file(t_tree *node)
{
	int	ret;

	ret = 0;
	if (node->token && node->token->type == CHEVB)
		return (open_chevb(node));
	else if (node->token && (node->token->type == CHEVF || \
		(node->token->type == FRED && ft_strcmp(node->right->cmds[0], "-"))))
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
	if (shell->fd_in != -1)
		if ((ret = pipe(fd)) == -1)
			ft_putstr("pipe failure !\n");
	return (ret);
}
