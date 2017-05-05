#include "21sh.h"

void	manage_pipe(char **cmd, int *fd_in)
{
				int		fd[2];
				pid_t	pid;

								(void)cmd;
				pipe(fd);
				if ((pid = fork()) == -1)
					ft_exiterror("fork failure !", -1);
				if (!pid)
				{
								dup2(*fd_in, 0);
								dup2(fd[1], 1);
								close(fd[0]);
//								execve(cmd[0], cmd, env);
								exit(0);
				}
				else
				{
								wait(NULL);
								close(fd[1]);
								*fd_in = fd[0];
				}
}

void	operators(t_tree *node, int *fd_in)
{
	if (node->token->type == PIPE)
	{
		if (!node->left->token)
			manage_pipe(list_to_tabstr(node->left->tokens), fd_in);
		if (!node->right->token)
			manage_pipe(list_to_tabstr(node->right->tokens), fd_in);
	}
	else
		*fd_in = 0;
}
