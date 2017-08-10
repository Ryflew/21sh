/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binary.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/17 23:39:09 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/08/10 03:07:22 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

static void	close_dup_fd(t_fd *fd, t_tree *node)
{
	if (fd->file == -2)
	{
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

void		manage_child_fd(t_sh *shell, t_tree *node, int *pipe)
{
	t_fd	*fd;
	t_list	*tmp;

	close(pipe[0]);
	child(node, shell, pipe);
	tmp = shell->fds_out;
	while (tmp && node->parent && (node->parent->token->type != PIPE
	|| shell->right_side))
	{
		fd = tmp->data;
		if (fd->type != FRED || fd->from != -1)
			dup2(fd->file, fd->from);
		else
			close_dup_fd(fd, node);
		close(fd->file);
		tmp = tmp->next;
	}
	if (node->from_fd != -1 && node->to_fd != -1)
	{
		dup2(node->to_fd, node->from_fd);
		close(node->to_fd);
	}
}

static char	execve_cmds(t_sh *shell, t_tree *node, t_env *env)
{
	char	*path;
	char	**envi;
	int		ret;

	if ((path = get_path(node, env, shell)))
	{
		envi = conv_env(env);
		ret = execve(path, node->cmds, envi);
		free(path);
		if (envi)
			ft_strdelpp(&envi);
	}
	else if (!shell->have_write_error)
	{
		ft_fputstr("21sh: command not found: ", 2);
		ft_fputendl(node->cmds[0], 2);
		ret = EXIT_FAILURE;
	}
	else
		ret = EXIT_FAILURE;
	return (ret);
}

char		run_binary(t_tree *node, t_env *env, t_sh *shell)
{
	int		pipe[2];
	int		ret;

	set_old_term(shell);
	if ((ret = get_fd(shell, pipe)) != -1)
	{
		if ((g_father = fork()) == -1)
			ft_exiterror("fork failure !", -1);
		else if (!g_father)
		{
			ret = EXIT_SUCCESS;
			manage_child_fd(shell, node, pipe);
			if (node->cmds)
				ret = execve_cmds(shell, node, env);
			exit(ret);
		}
		else
			ret = father(shell, pipe);
	}
	set_our_term(shell);
	return (WEXITSTATUS(ret));
}

char		run_builtins(t_tree *node, t_env **env, t_sh *shell)
{
	int		ret;
	int		pipe[2];

	set_old_term(shell);
	if ((ret = get_fd(shell, pipe)) != -1)
	{
		if ((g_father = fork()) == -1)
			ft_exiterror("fork failure !", -1);
		else if (!g_father)
		{
			ret = EXIT_SUCCESS;
			manage_child_fd(shell, node, pipe);
			if (node->cmds)
				ret = go_builtins(node->cmds, env, shell);
			exit(ret);
		}
		else
			ret = father(shell, pipe);
	}
	set_our_term(shell);
	return (WEXITSTATUS(ret));
}
