/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   father.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/10 20:25:08 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/10/10 20:25:09 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

pid_t	g_father = -1;

int		father(t_sh *shell, int *fd, t_tree *node)
{
	int ret;

	close(fd[1]);
	if (shell->fd_pipe != -1)
	{
		if (shell->fd_pipe)
			close(shell->fd_pipe);
		shell->fd_pipe = fd[0];
		if (shell->right_side)
		{
			waitpid(g_father, &ret, 0);
			close(fd[0]);
			shell->fd_pipe = -1;
		}
		else
			waitpid(g_father, &ret, WNOHANG);
	}
	else
		waitpid(g_father, &ret, 0);
	if (node && node->TYPE == DCHEVB)
		close(fd[0]);
	g_father = -1;
	shell->return_value = WEXITSTATUS(ret);
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
	ft_strdelpp(&node->cmds);
	node->cmds = tab;
	return (is_absolute(node, env, shell));
}
