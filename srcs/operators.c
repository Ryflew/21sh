#include "21sh.h"

char manage_pipe(t_env **env, t_sh *shell, t_tree *node, int *fd_in, char right)
{
	int fd[2];
	pid_t pid;

	ft_putstr("pipe\n");
	pipe(fd);
	if ((pid = fork()) == -1)
		ft_exiterror("fork failure !", -1);
	if (!pid)
	{
		dup2(*fd_in, 0);
		if (!right || (node->parent->parent && node->parent->parent->token->type == PIPE))
			dup2(fd[1], 1);
		else
			*fd_in = -1;
		close(fd[0]);
		exec_cmds(list_to_tabstr(node->tokens), env, shell, 1);
		exit(0);
	}
	else
	{
		wait(NULL);
		//g_father = -1;
		close(fd[1]);
		*fd_in = fd[0];
	}
	return (1); // TODO
}

void manage_and(t_env **env, t_sh *shell, t_tree *node, int *fd_in)
{
	if (*fd_in != -1)
	{
		if (manage_pipe(env, shell, node->left, fd_in, 0))
			manage_pipe(env, shell, node->right, fd_in, 1);
	}
	else
		if (exec_cmds(list_to_tabstr(node->left->tokens), env, shell, 0))
			exec_cmds(list_to_tabstr(node->right->tokens), env, shell, 0);
}

void manage_or(t_env **env, t_sh *shell, t_tree *node, int *fd_in)
{
	if (*fd_in != -1)
	{
		if (!manage_pipe(env, shell, node->left, fd_in, 0))
			manage_pipe(env, shell, node->right, fd_in, 1);
	}
	else
		if (!exec_cmds(list_to_tabstr(node->left->tokens), env, shell, 0))
			exec_cmds(list_to_tabstr(node->right->tokens), env, shell, 0);
}

void operators(t_tree *node, int *fd_in, t_env **env, t_sh *shell)
{
	if (node->token->type == PIPE)
	{
		if (*fd_in == -1)
			*fd_in = 0;
		if (!node->left->token)
			manage_pipe(env, shell, node->left, fd_in, 0);
		if (!node->right->token)
			manage_pipe(env, shell, node->right, fd_in, 1);
	}
	else if (node->token->type == AND)
		manage_and(env, shell, node, fd_in);
	else if (node->token->type == OR)
		manage_or(env, shell, node, fd_in);
	else
		*fd_in = -1;
}
