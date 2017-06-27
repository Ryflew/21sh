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

char	run_binary(char *path, t_tree *node, t_env *env, t_sh *shell)
{
	int		pipe[2];
	char	**envi;
	int		ret;
	t_list	*tmp;
	t_fd	*fd1;
	//t_fd	*fd2;
	int		tmp_fd;
	
	set_old_term(shell);
	if ((ret = get_fd(shell, pipe)) != -1)
	{
		if ((g_father = fork()) == -1)
			ft_exiterror("fork failure !", -1);
		else if (!g_father)
		{
			envi = conv_env(env);
			child(node, shell, pipe);
			tmp = shell->fds_out;
			while (tmp && node->parent && (node->parent->token->type != PIPE || shell->right_side))
			{
			//if (shell->fds_out && node->parent && (node->parent->token->type != PIPE || shell->right_side))
			//{
				fd1 = tmp->data;
				ft_putstr("fd1 = ");
				ft_putnbr(fd1->file);
				ft_putendl("");
				ft_putstr("from1 = ");
				ft_putnbr(fd1->from);
				ft_putendl("");	
				/*if (tmp->next)
				{
					fd2 = tmp->next->data;
					ft_putstr("fd2 = ");
					ft_putnbr(fd2->file);
					ft_putendl("");
					ft_putstr("from2 = ");
					ft_putnbr(fd2->from);
					ft_putendl("");
					if (fd1->from == fd2->from)
					{
					ft_putstr("equal\n");						
						dup2(fd1->file, fd2->file);
					}
					else
					{
						ft_putstr("from1 = ");
					ft_putnbr(fd2->from);
					ft_putendl("");
					ft_putstr("not equal\n");					
						dup2(fd1->file, fd1->from);
					}
				}
				else
				{*/
					ft_putstr("else\n");
					close(fd1->file);
					tmp_fd = dup(fd1->from);		
					dup2(tmp_fd, fd1->from);
				//}
				tmp = tmp->next;
			}
			if (node->from_fd != -1 && node->to_fd != -1)
				dup2(node->to_fd, node->from_fd);
			if (node->cmds)
				execve(path, node->cmds, envi);
			if (envi)
				ft_strdelpp(&envi);
			exit(EXIT_SUCCESS);
		}
//				tmp = tmp->next;
//			}
		else
			ret = father(shell, pipe);
	}
	set_our_term(shell);
	free(path);
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
				dup2(fd->file, 1);
				tmp = tmp->next;
			}
			if (node->from_fd != -1 && node->to_fd != -1)
				dup2(node->to_fd, node->from_fd);
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
	i = is_absolute(node, env, shell, 1);
	ft_strdelpp(&node->cmds);
	return (i);
}
