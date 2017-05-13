/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binary.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/17 23:39:09 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/05/04 20:26:51 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include "21sh.h"

static pid_t	g_father = -1;

char	stop_binary(int sig)
{
	if (g_father != -1 && sig == SIGINT)
	{
		kill(g_father, sig);
		ft_putchar('\n');
		return (1);
	}
	return (0);
}

char **run_with_pipe(t_tree *node, t_sh *shell, int *fd)
{
	if (node->from_fd != -1 && node->to_fd != -1)
		dup2(fd[1], node->from_fd);
	dup2(shell->fd_in, 0);
	if (!shell->right_side)
		dup2(fd[1], 1);
	close(fd[0]);
	return (node->cmds);
}

char	**child(t_tree *node, t_sh *shell, int *fd, int fd_file)
{
	char	**cmds;
		
	cmds = NULL;
	if (node->token && node->token->type == DCHEVB)
		cmds = manage_dchevb(node);
	else if (node->token && node->token->type == CHEVB)
		cmds = manage_chevb(node, fd_file);
	else if (node->token && node->token->type == DCHEVF)
		cmds = manage_dchevf(node, fd_file);
	else if (node->token && node->token->type == CHEVF)
		cmds = manage_chevf(node, fd_file);
	else if (node->token && node->token->type == FRED)
		cmds = manage_fred(node, fd_file);
	else if (shell->fd_in != -1)
		cmds = run_with_pipe(node, shell, fd);
	else
	{
		if (node->from_fd != -1 && node->to_fd != -1)
			dup2(node->from_fd, node->to_fd);
		else if (node->to_fd != -1)
			dup2(1, node->to_fd);
		cmds = node->cmds;
	}
	return (cmds);
}

int		open_file(t_tree *node, t_sh *shell, int *fd)
{
	if (shell->fd_in != -1)
		return (pipe(fd));
	if (node->token && node->token->type == CHEVB)
		return (open_chevb(node));
	else if (node->token && (node->token->type == CHEVF || (node->token->type == FRED && ft_strcmp(node->right->cmds[0], "-"))))
		return (open_chevf(node));
	else if (node->token && node->token->type == DCHEVF)
		return (open_dchevf(node));
	else
		return (0);
}

int		father(t_sh *shell, int *fd)
{
	int ret;

	wait(&ret);	
	if (shell->fd_in != -1)
	{
		close(fd[1]);
		shell->fd_in = fd[0];
		if (shell->right_side)
			shell->fd_in = -1;
	}
	g_father = -1;
	return (ret);
}

char	run_binary(char *path, t_tree *node, t_env *env, t_sh *shell)
{
	int		ret;
	int		fd[2];
	char	**envi;
	char	**cmds;
	
	if ((ret = open_file(node, shell, fd)) == -1)
		return (-1);
	if ((g_father = fork()) == -1)
		ft_exiterror("fork failure !", -1);	
	else if (!g_father)
	{
		envi = conv_env(env);
		if ((cmds = child(node, shell, fd, ret)))
			execve(path, cmds, envi);
		if (envi)
			ft_strdelpp(&envi);
		exit(EXIT_SUCCESS);
	}
	else
		ret = father(shell, fd);
	free(path);
	return (WEXITSTATUS(ret));		
}

char	current_binary(t_tree *node, t_env *env, t_sh *shell)
{
	int		i;
	char	*str;
	char	*cwd;
	char	**tab;
	char	buff[4097];

	str = ft_strsub(*node->cmds, 2, ft_strlen(*node->cmds) - 2);
	i = -1;
	while (node->cmds[++i])
		;
	if (!(tab = (char**)malloc(sizeof(char*) * (i + 1))))
		exit(EXIT_FAILURE);
	if (!(cwd = getcwd(buff, 4097)))
		return (0);
	tab[0] = ft_strstrjoin(cwd, "/", str);
	free(str);
	i = 0;
	while (node->cmds[++i])
		tab[i] = ft_strdup(node->cmds[i]);
	tab[i] = NULL;
	i = is_absolute(node, env, shell);
	ft_strdelpp(&node->cmds);
	return (i);
}
