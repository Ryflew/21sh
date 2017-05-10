#include "21sh.h"

static char manage_pipe(t_env **env, t_sh *shell, t_tree *node, char right_side)
{
	int fd[2];
	pid_t pid;
	int	ret;

	pipe(fd);
	ret = 0;
	if ((pid = fork()) == -1)
		ft_exiterror("fork failure !", -1);
	if (!pid)
	{
		dup2(shell->pipe->fd_in, 0);
		if (!right_side)
			dup2(fd[1], 1);
		close(fd[0]);
		exec_cmds(node->cmds, env, shell, 1);
		exit(0);
	}
	else
	{
		wait(&ret);
		if (right_side)
			shell->pipe->fd_in = -1;
		//g_father = -1;
		close(fd[1]);
		shell->pipe->fd_in = fd[0];
	}
	return (WEXITSTATUS(ret));
}

static void manage_and(t_env **env, t_sh *shell, t_tree *node, char right_side)
{
	int ret; 

	if (shell->pipe->fd_in != -1)
	{
		if (!is_builtins(node->left->cmds))
		{
			if (!go_builtins(node->left->cmds, env, shell))
				manage_pipe(env, shell, node->right, right_side);
		}
		else if (get_path(node->left->cmds, *env, 0, 0))
		{
			ft_putstr(node->left->cmds[0]);
			ft_putendl(": Command not found.");
		}
		else
		{
			if (!(ret = manage_pipe(env, shell, node->left, 0)))
				manage_pipe(env, shell, node->right, right_side);
		}
	}
	else
		if (!exec_cmds(node->left->cmds, env, shell, 0))
			exec_cmds(node->right->cmds, env, shell, 0);
}

static void manage_or(t_env **env, t_sh *shell, t_tree *node, char right_side)
{
	if (shell->pipe->fd_in != -1)
	{
		if (!is_builtins(node->left->cmds))
		{
			if (go_builtins(node->left->cmds, env, shell))
				manage_pipe(env, shell, node->right, right_side);
		}
		else if (get_path(node->left->cmds, *env, 0, 0))
		{
			ft_putstr(node->left->cmds[0]);
			ft_putendl(": Command not found.");
			manage_pipe(env, shell, node->right, right_side);			
		}
		else
			if (manage_pipe(env, shell, node->left, 0))
				manage_pipe(env, shell, node->right, right_side);
	}
	else
		if (exec_cmds(node->left->cmds, env, shell, 0))
			exec_cmds(node->right->cmds, env, shell, 0);
}

void	init_pipe(t_sh *shell)
{
	if (!(shell->pipe = (t_pipe*)malloc(sizeof(t_pipe))))
		ft_exiterror("Malloc failure ", -1);
	shell->pipe->fd_in = -1; 
}

void	operators(t_tree *node, t_env **env, t_sh *shell, char right_side)
{
	if (node->token->type == PIPE)
	{
		if (!shell->pipe)
			init_pipe(shell);
		if (shell->pipe->fd_in == -1)
			shell->pipe->fd_in = 0;
		if (!node->left->token)
			manage_pipe(env, shell, node->left, 0);
		if (!node->right->token)
			manage_pipe(env, shell, node->right, right_side);
	}
	else if (node->token->type == AND)
		manage_and(env, shell, node, right_side);
	else if (node->token->type == OR)
		manage_or(env, shell, node, right_side);
}
