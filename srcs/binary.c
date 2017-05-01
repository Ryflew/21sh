/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binary.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/17 23:39:09 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/03/17 23:50:28 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
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

void	run_binary(char *path, char **av, t_env *env)
{
	char	**envi;

	g_father = fork();
	if (g_father > 0)
	{
		wait(NULL);
		g_father = -1;
	}
	else if (!g_father)
	{
		envi = conv_env(env);
		execve(path, av, envi);
		if (envi)
			ft_strdelpp(&envi);
		exit(EXIT_SUCCESS);
	}
	g_father = -1;
	free(path);
}
