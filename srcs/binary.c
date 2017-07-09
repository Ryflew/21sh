/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binary.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/17 23:39:09 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/06/19 23:22:16 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

static pid_t	g_father = -1;

pid_t	child_pid(void)
{
	return (g_father);
}

int		father(t_sh *shell, int *fd)
{
	int ret;

	waitpid(g_father, &ret, 0);
	if (shell->fd_pipe != -1)
	{
		close(fd[1]);
		shell->fd_pipe = fd[0];
		if (shell->right_side)
			shell->fd_pipe = -1;
	}
	g_father = -1;
	return (ret);
}

char	run_binary(t_tree *node, t_env *env, t_sh *shell)
{
	int		pipe[2];
	char	**envi;
	int		ret;
	t_list	*tmp;
	t_fd	*fd;
	char	*path;
	
	set_old_term(shell);
	if ((ret = get_fd(shell, pipe)) != -1)
	{
				//ft_putendl("before");		
		if ((g_father = fork()) == -1)
			ft_exiterror("fork failure !", -1);
		else if (!g_father)
		{
				//ft_fputendl("sun", 2);					
			ret = EXIT_SUCCESS;
			child(node, shell, pipe);
				//ft_fputendl("problemes", 2);					
			tmp = shell->fds_out;
			while (tmp && node->parent && (node->parent->token->type != PIPE || shell->right_side))
			{
				//ft_fputendl("fds_out", 2);
				fd = tmp->data;
				if (fd->type != FRED || fd->from != -1)
					dup2(fd->file, fd->from);
				else
				{
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
					else if (fd->from == -1 && fd->to == -1)
					{
						dup2(fd->file, 1);
						dup2(fd->file, 2);
					}
				}
				close(fd->file);
				tmp = tmp->next;								
			}
			if (node->from_fd != -1 && node->to_fd != -1)
			{
				//ft_fputendl("from to", 2);				
				dup2(node->to_fd, node->from_fd);
				close(node->to_fd);
			}
			if (node->cmds)
			{
				if ((path = get_path(node, env, shell)))
				{
					envi = conv_env(env);
					for (int i = 0; envi[i]; i++)
					{
						ft_fputendl(envi[i], 2);
					}
					//ft_fputendl("exec", 2);									
					ret = execve(path, node->cmds, envi);
					free(path);
					if (envi)
						ft_strdelpp(&envi);
				}
				else
				{
					ft_fputstr("21sh: command not found: ", 2);
					ft_fputendl(node->cmds[0], 2);
					ret = EXIT_FAILURE;
				}
			}
			return (ret);//exit(EXIT_SUCCESS);
		}
		else
		{
				ft_putendl("sun");					
			ret = father(shell, pipe);
		}
	}
	set_our_term(shell);
	return (WEXITSTATUS(ret));
}

char	run_builtins(t_tree *node, t_env **env, t_sh *shell)
{
	int		ret;
	int		pipe[2];
	char	**envi;
	t_list	*tmp;
	t_fd	*fd;

	set_old_term(shell);
	if ((ret = get_fd(shell, pipe)) != -1)
	{
		if ((g_father = fork()) == -1)
			ft_exiterror("fork failure !", -1);
		//else if (!g_father && (node->cmds || node->token->type == CHEVB || \
		//	node->token->type == DCHEVB))
		else if (!g_father)
		{
			envi = conv_env(*env);
			child(node, shell, pipe);
			tmp = shell->fds_out;
			while (tmp && node->parent && (node->parent->token->type != PIPE || shell->right_side))
			{
				fd = tmp->data;
				if (fd->type != FRED || fd->from != -1)
					dup2(fd->file, fd->from);
				else
				{
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
					else if (fd->from == -1 && fd->to == -1)
					{
						dup2(fd->file, 1);
						dup2(fd->file, 2);
					}
				}
				close(fd->file);
				tmp = tmp->next;								
			}
			if (node->from_fd != -1 && node->to_fd != -1)
			{
				dup2(node->to_fd, node->from_fd);
				close(node->to_fd);
			}
			if (node->cmds)
				go_builtins(node->cmds, env, shell);
			if (envi)
				ft_strdelpp(&envi);
			exit(EXIT_SUCCESS);
		}
		else
			ret = father(shell, pipe);
	}
	set_our_term(shell);
	return (WEXITSTATUS(ret));
}

char	*current_binary(t_tree *node, t_env *env, t_sh *shell)
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
	str = is_absolute(node, env, shell);
	ft_strdelpp(&node->cmds);
	return (str);
}
