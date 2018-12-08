/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   father.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/10 20:25:08 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/12/06 17:35:20 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

pid_t	g_father = -1;

static void	wait_if_pipe(t_sh *shell, int *ret, int *fd, t_tree *node)
{
	if (shell->fd_pipe)
		close(shell->fd_pipe);
	if ((shell->fd_pipe = fd[0]) == fd[0] && shell->right_side)
	{
		close(fd[0]);
		shell->fd_pipe = -1;
		waitpid(g_father, ret, 0);
	}
	if (node && node->token && node->TYPE != DCHEVB)
		waitpid(g_father, ret, WNOHANG);
	else
		waitpid(g_father, ret, 0);
}

int			father(t_sh *shell, int *fd, int *heredoc_pipe, t_tree *node)
{
	int ret;

	add_pid(&shell->lst_pid, g_father);
	close(fd[1]);
	if (node && node->TYPE == DCHEVB)
		close(heredoc_pipe[1]);
	if (shell->fd_pipe != -1)
		wait_if_pipe(shell, &ret, fd, node);
	else
		waitpid(g_father, &ret, 0);
	if (node && node->TYPE == DCHEVB)
		close(heredoc_pipe[0]);
	g_father = -1;
	shell->return_value = WEXITSTATUS(ret);
	return (WEXITSTATUS(ret));
}

char		*current_binary(t_tree *node, t_env *env, t_sh *shell)
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
		ft_exiterror("Malloc failed", 1);
	if (!(cwd = getcwd(buff, 4097)))
		return (0);
	tab[0] = ft_strstrjoin(cwd, "/", str);
	free(str);
	i = 0;
	while (node->cmds[++i])
		tab[i] = ft_strdup(node->cmds[i]);
	tab[i] = NULL;
	ft_strdelpp(&node->cmds);
	node->cmds = tab;
	return (is_absolute(node, env, shell));
}
