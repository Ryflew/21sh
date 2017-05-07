#include "21sh.h"

char manage_pipe(t_env **env, t_sh *shell, char **cmd, int *fd_in)
{
	int fd[2];
	pid_t pid;

	(void)cmd;
	pipe(fd);
	if ((pid = fork()) == -1)
		ft_exiterror("fork failure !", -1);
	if (!pid)
	{
		dup2(*fd_in, 0);
		dup2(fd[1], 1);
		close(fd[0]);
		exec_cmds(cmd, env, shell);
		exit(0);
	}
	else
	{
		wait(NULL);
		close(fd[1]);
		*fd_in = fd[0];
	}
	return (1); // TODO
}

void manage_and(t_env **env, t_sh *shell, t_tree *node, int *fd_in)
{
	if (*fd_in != -1)
	{
		if (manage_pipe(env, shell, list_to_tabstr(node->left->tokens), fd_in))
			manage_pipe(env, shell, list_to_tabstr(node->right->tokens), fd_in);
	}
	else
		if (exec_cmds(list_to_tabstr(node->left->tokens), env, shell))
			exec_cmds(list_to_tabstr(node->right->tokens), env, shell);
}

void manage_or(t_env **env, t_sh *shell, t_tree *node, int *fd_in)
{
	if (*fd_in != -1)
	{
		if (!manage_pipe(env, shell, list_to_tabstr(node->left->tokens), fd_in))
			manage_pipe(env, shell, list_to_tabstr(node->right->tokens), fd_in);
	}
	else
		if (!exec_cmds(list_to_tabstr(node->left->tokens), env, shell))
			exec_cmds(list_to_tabstr(node->right->tokens), env, shell);
}

void operators(t_tree *node, int *fd_in, t_env **env, t_sh *shell)
{
	if (node->token->type == PIPE)
	{
		if (!node->left->token)
			manage_pipe(env, shell, list_to_tabstr(node->left->tokens), fd_in);
		if (!node->right->token)
			manage_pipe(env, shell, list_to_tabstr(node->right->tokens), fd_in);
	}
	else if (node->token->type == AND)
		manage_and(env, shell, node, fd_in);
	else if (node->token->type == OR)
		manage_or(env, shell, node, fd_in);
	else if (node->parent_type != PIPE)
		*fd_in = -1;
}
