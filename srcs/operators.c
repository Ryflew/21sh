#include "21sh.h"
#include <fcntl.h>

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
	{
		dup2(fd_pipe[0], 0);
		close(fd_pipe[0]);
	}
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
				close(fd->file);				
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
	{
		dup2(fd.to, 0);
		close(fd.to);
	}
	else
	{
		dup2(fd.file, 0);
		close(fd.file);
	}
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
	}
	else if (node->token->type == CHEVB)
	{
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
