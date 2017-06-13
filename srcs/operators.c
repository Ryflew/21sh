#include "21sh.h"
#include <fcntl.h>

static char manage_pipe(t_env **env, t_sh *shell, t_tree *node, char right_side)
{
	shell->right_side = right_side;
	return (exec_cmds_with_op(node, env, shell));
}

char	**manage_dchevb(t_tree *node)
{
	int		fd[2];
	char	*line;
	
	line = NULL;
	pipe(fd);
	while (gnl(0, &line) && ft_strcmp(line, node->right->cmds[0]))
	{
		// ft_putendl(line);
		ft_fputendl(line, fd[1]);
	}
	if (node->to_fd != -1)
		dup2(fd[0], node->to_fd);
	else
		dup2(fd[0], 0);
	close(fd[1]);
	if (node->from_fd != -1 && node->to_fd != -1)
		dup2(node->from_fd, node->to_fd);
	else if (node->to_fd != -1)
		dup2(1, node->to_fd);
	return (node->left->cmds);
}

int	open_chevb(t_tree *node)
{
	int		fd_file;	

	if ((fd_file = open(node->right->cmds[0], O_RDONLY)) == -1)
	{
		ft_putstr("Open failure -> ");
		ft_putendl(node->right->cmds[0]);
	}
	return (fd_file);
}

char	**manage_chevb(t_tree *node, int fd_file)
{
	if (node->to_fd != -1)
		dup2(fd_file, node->to_fd);
	else
		dup2(fd_file, 0);
	if (node->from_fd != -1 && node->to_fd != -1)
		dup2(node->from_fd, node->to_fd);
	else if (node->to_fd != -1)
		dup2(1, node->to_fd);
	close(fd_file);
	return (node->left->cmds);
}

int	open_dchevf(t_tree *node)
{
	int		fd_file;
	
	if ((fd_file = open(node->right->cmds[0], O_WRONLY | O_APPEND | O_CREAT, 0644)) == -1)
	{
		ft_putstr("Open failure -> ");
		ft_putendl(node->right->cmds[0]);
	}
	return (fd_file);
}

char	**manage_dchevf(t_tree *node, int fd_file)
{
	if (node->from_fd != -1)
		dup2(fd_file, node->from_fd);
	else
		dup2(fd_file, 1);
	if (node->from_fd != -1 && node->to_fd != -1)
		dup2(node->from_fd, node->to_fd);
	else if (node->to_fd != -1)
		dup2(1, node->to_fd);
	close(fd_file);	
	return (node->left->cmds);
}

int	open_chevf(t_tree *node)
{
	int		fd_file;
	
	if ((fd_file = open(node->right->cmds[0], O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1)
	{
		ft_putstr("Open failure -> ");
		ft_putendl(node->right->cmds[0]);
	}
	return (fd_file);
}

char	**manage_chevf(t_tree *node, int fd_file)
{
	if (node->from_fd != -1)
		dup2(fd_file, node->from_fd);
	else
		dup2(fd_file, 1);		
	if (node->from_fd != -1 && node->to_fd != -1)
		dup2(node->from_fd, node->to_fd);
	else if (node->to_fd != -1)
		dup2(1, node->to_fd);
	close(fd_file);
	return (node->left->cmds);
}

/*static void	manage_bred(t_tree *node)
{
	if (!ft_strcmp(node->right->cmds[0], "-"))
	{
		if (node->to_fd)				
			close(node->to_fd);
		else
			close(0);
	}
	else
	{
		if (node->to_fd) // TODO test valid fd
			dup2(node->from_fd, node->to_fd);
		else
			dup2(node->from_fd, 0);
	}
}*/

char	**manage_fred(t_tree *node, int fd_file)
{
	if (!ft_strcmp(node->right->cmds[0], "-"))
	{
		// TODO test valid fd
		if (node->from_fd != -1)				
			close(node->from_fd);
		else
			close(1);
		return (NULL);
	}
	/*else if (node->from_fd != -1 && node->to_fd != -1)
	{
		if (node->from_fd)		
			dup2(node->from_fd, node->to_fd);
		else
			dup2(1, node->to_fd);
	}*/
	else
	{
		if (node->from_fd == -1 && node->to_fd == -1)
		{
			dup2(fd_file, 1);
			dup2(fd_file, 2);
		}
		else if (node->to_fd == -1)
			dup2(fd_file, node->from_fd); // TODO test valid fd
		close(fd_file);		
		return (node->left->cmds);
	}
}

static char manage_and(t_env **env, t_sh *shell, t_tree *node, char right_side)
{
	int ret; 

	ret = 0;
	if (shell->fd_in != -1)
	{
		if (!is_builtins(node->left->cmds))
		{
			if (!go_builtins(node->left->cmds, env, shell))
				manage_pipe(env, shell, node->right, right_side);
		}
		else if (get_path(node->left, *env, shell, 0))
		{
			ft_putstr(node->left->cmds[0]);
			ft_putendl(": Command not found.");
			return (-1);
		}
		else
		{
			if (!(ret = manage_pipe(env, shell, node->left, 0)))
				manage_pipe(env, shell, node->right, right_side);
			else if (ret == -1)
				shell->fd_in = -1;				
		}
	}
	else
	{
		if (node->from_fd != -1 && node->to_fd != -1)
			dup2(node->from_fd, node->to_fd);
		if (!(ret = exec_cmds(node->left, env, shell)))
			return (exec_cmds(node->right, env, shell));
	}
	return (ret);
}

static char manage_or(t_env **env, t_sh *shell, t_tree *node, char right_side)
{
	int ret;

	ret = 0;
	if (shell->fd_in != -1)
	{
		if (!is_builtins(node->left->cmds))
		{
			if (go_builtins(node->left->cmds, env, shell))
				return (manage_pipe(env, shell, node->right, right_side));
		}
		else if (get_path(node->left, *env, shell, 0))
		{
			ft_putstr(node->left->cmds[0]);
			ft_putendl(": Command not found.");
			shell->fd_in = -1;
			return (-1);
			//manage_pipe(env, shell, node->right, right_side);
		}
		else
		{
			if ((ret = manage_pipe(env, shell, node->left, 0)) != 0 && ret != -1)
				return (manage_pipe(env, shell, node->right, right_side));
			else if (ret == -1)
				shell->fd_in = -1;				
		}
	}
	else
	{
		if (node->from_fd != -1 && node->to_fd != -1)
			dup2(node->from_fd, node->to_fd);
		if ((ret = exec_cmds(node->left, env, shell)) != 0 && ret != -1)
			return (exec_cmds(node->right, env, shell));
	}
	return (ret);
}

char	operators(t_tree *node, t_env **env, t_sh *shell, char right_side)
{
	int	ret;

	ret = 0;
	if (node->token->type == PIPE)
	{
		if (shell->fd_in == -1)
			shell->fd_in = 0;
		if (!node->left->token && (ret = manage_pipe(env, shell, node->left, 0)) == -1)
			return (-1);
		if (!node->right->token)
			ret = manage_pipe(env, shell, node->right, right_side);
		return (ret);
	}
	else if (node->token->type == AND)
		return (manage_and(env, shell, node, right_side));
	else if (node->token->type == OR)
		return (manage_or(env, shell, node, right_side));
	else if ((node->token->type == CHEVB || node->token->type == DCHEVB || node->token->type == CHEVF || node->token->type == DCHEVF || node->token->type == FRED))
	{
		if (node->left)
			node->cmds = node->left->cmds;
		else
			node->cmds = NULL;
		return (exec_cmds_with_op(node, env, shell));
	}
	return (0);
}
