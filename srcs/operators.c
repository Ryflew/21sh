#include "21sh.h"
#include <fcntl.h>

/*static char	manage_pipe(t_env **env, t_sh *shell, t_tree *node, char right_side)
{
	shell->right_side = right_side;
	return (exec_cmds_with_op(node, env, shell));
}*/

void	manage_dchevb(t_tree *node, char *cmd, t_list *fds_out)
{
	int		fd_pipe[2];
	char	*line;
	t_list	*tmp;
	t_fd	*fd;

	line = NULL;
	pipe(fd_pipe);
	ft_putstr("heredoc> ");
	while (gnl(0, &line) && ft_strcmp(line, cmd))
	{
		ft_putstr("heredoc> ");
		ft_fputendl(line, fd_pipe[1]);
		free(line);
	}
	free(line);
	close(fd_pipe[1]);
	if (node->cmds)
		dup2(fd_pipe[0], 0);
	else
	{
		if (fds_out)
		{
			tmp = fds_out;
			//call function of binairy
			while (tmp)
			{
				fd = tmp->data;
				dup2(fd->file, fd->from);
				tmp = tmp->next;
			}
		}
		while (gnl(fd_pipe[0], &line))
		{
			if (node->to_fd != -1)
				ft_fputendl(line, node->to_fd);
			else
				ft_putendl(line);
			free(line);			
		}
		free(line);
	}
}

int			open_chevb(t_tree *node)
{
	int		fd_file;

	if ((fd_file = open(node->right->cmds[0], O_RDONLY)) == -1)
	{
		ft_putstr("Open failure -> ");
		ft_putendl(node->right->cmds[0]);
	}
	return (fd_file);
}

void		manage_chevb(t_fd fd)
{	
	if (fd.to != -1)
		dup2(fd.to, 0);
	else
		dup2(fd.file, 0);
	close(fd.file);
}

int			open_dchevf(t_tree *node)
{
	int		fd_file;

	if ((fd_file = open(node->right->cmds[0], O_WRONLY | O_APPEND | \
		O_CREAT, 0644)) == -1)
	{
		ft_putstr("Open failure -> ");
		ft_putendl(node->right->cmds[0]);
	}
	return (fd_file);
}

int			open_chevf(t_tree *node)
{
	int		fd_file;

	if ((fd_file = open(node->right->cmds[0], O_WRONLY | O_CREAT | \
		O_TRUNC, 0644)) == -1)
	{
		ft_putstr("Open failure -> ");
		ft_putendl(node->right->cmds[0]);
	}
	return (fd_file);
}

// void	manage_chevf(t_list	*fds_out)
// {
// 	t_list	*tmp;
// 	t_fd	*fd;

// 	tmp = fds_out;
// 	while (tmp)
// 	{
// 		fd = tmp->data;
		
// 	}
// 	//if (node->from_fd != -1 && node->to_fd != -1)
// 	//	dup2(node->from_fd, node->to_fd);
// 	//else if (node->to_fd != -1)
// 	//	dup2(1, node->to_fd);
// 	close(fd->file);
// }

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

/*void		manage_fred(t_tree *node, t_list *fds_out)
{
	t_list	*tmp;
	t_fd	*fd;

	tmp = fds_out;
	while (tmp)
	{
		fd = tmp->data;
		if (fd->file == -2)
		{
			// TODO test valid fd
			if (fd->from != -1)
				close(fd->from);
			else
				close(1);
			ft_free_tab(node->cmds);
			node->cmds = NULL;
		}
	else if (node->from_fd != -1 && node->to_fd != -1)
	{
		if (node->from_fd)
			dup2(node->from_fd, node->to_fd);
		else
			dup2(1, node->to_fd);
	}
		else
		{
			if (fd->from == -1 && fd->to == -1)
			{
				dup2(fd->file, 1);
				dup2(fd->file, 2);
				ft_putendl("FUCK");
			}
			else if (node->from_fd != -1)
				dup2(fd->file, fd->from); // TODO test valid fd
			close(fd->file);
		}
	}
}*/

/*static char	manage_and(t_env **env, t_sh *shell, t_tree *node, char right_side)
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
			return (-1);
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

static char	manage_or(t_env **env, t_sh *shell, t_tree *node, char right_side)
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
}*/

char		operators(t_tree *node, t_sh *shell)
{
	int		ret;
	char	**new_cmd;
	char	**envi;
	t_fd	*fd;

	ret = 0;
	if (node->token->type == PIPE)
	{
		if (shell->fd_pipe == -1)
			shell->fd_pipe = 0;
		//if (!node->left->token && (ret = manage_pipe(env, shell, node->left, 0)) == -1)
			//return (-1);
		//if (!node->right->token)
			//ret = manage_pipe(env, shell, node->right, right_side);
		//return (ret);
	}
	//else if (node->token->type == AND)
	//	return (manage_and(env, shell, node, right_side));
	//else if (node->token->type == OR)
	//	return (manage_or(env, shell, node, right_side));
	else if (node->token->type == CHEVB)
	{
		/*if (node->left)
			node->cmds = node->left->cmds;
		else
			node->cmds = NULL;
		return (exec_cmds_with_op(node, env, shell));*/
		if (!node->left && node->to_fd == -1)
		{
			envi = conv_env(shell->env);
			new_cmd = (char**)malloc(sizeof(char*) * 3);
			new_cmd[2] = NULL;
			new_cmd[0] = ft_strdup("/bin/cat");
			new_cmd[1] = ft_strdup(node->right->cmds[0]);
			execve(new_cmd[0], new_cmd, envi);
			ft_strdelpp(&envi);
		}
		else if ((shell->fd_in.file = open_file(node)) == -1)
			return (-1);
		shell->fd_in.to = node->to_fd;
		shell->fd_in.from = -1;
		shell->fd_in.type = node->token->type;		
	}
	else if (node->token->type == DCHEVB && !node->left)
		manage_dchevb(node, node->right->cmds[0], shell->fds_out);
	else if (node->token->type == CHEVF || node->token->type == DCHEVF || node->token->type == FRED)
	{
		if (!(fd = (t_fd*)malloc(sizeof(t_fd))))
			exit(-1);
		if (node->token->type == FRED && !ft_strcmp(node->right->cmds[0], "-"))
			fd->file = -2;
		else
			if ((fd->file = open_file(node)) == -1)
				return (-1);
		if (node->token->type != FRED)
			fd->from = (node->from_fd == -1) ? 1 : node->from_fd;
		else
			fd->from = node->from_fd;
		fd->to = -1;
		fd->type = node->token->type;
		ft_node_push_front(&shell->fds_out, fd);
	}
	return (0);
}
