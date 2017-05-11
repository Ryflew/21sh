/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binary.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/17 23:39:09 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/05/11 03:06:35 by vdarmaya         ###   ########.fr       */
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
		g_father = -1;
		ft_putchar('\n');
		return (1);
	}
	return (0);
}

char	run_binary(char *path, char **av, t_sh *shell, char pipe)
{
	char	**envi;

	if (tcsetattr(0, TCSADRAIN, &(shell->old)) == -1)
	{
		errexit("21sh", "Impossible de set l'ancien terminal");
		exit(EXIT_FAILURE);
	}
	if (!pipe)
	{
		g_father = fork();
		if (g_father > 0)
			wait(NULL);
		else if (g_father == 0)
		{
			envi = conv_env(shell->env);
			execve(path, av, envi);
			if (envi)
				ft_strdelpp(&envi);
			exit(EXIT_SUCCESS);
		}
		g_father = -1;
	}
	else
	{
		envi = conv_env(shell->env);
		execve(path, av, envi);
		if (envi)
			ft_strdelpp(&envi);
	}
	free(path);
	if (tcsetattr(0, TCSADRAIN, &(shell->our)) == -1)
	{
		errexit("21sh", "Impossible de set le nouveau terminal");
		exit(EXIT_FAILURE);
	}
	return (1);
}

static char	current_binary(char **av, t_sh *shell, char pipe)
{
	int		i;
	char	*str;
	char	*cwd;
	char	**tab;
	char	buff[4097];

	str = ft_strsub(*av, 2, ft_strlen(*av) - 2);
	i = -1;
	while (av[++i])
		;
	if (!(tab = (char**)malloc(sizeof(char*) * (i + 1))))
		exit(EXIT_FAILURE);
	if (!(cwd = getcwd(buff, 4097)))
		return (0);
	tab[0] = ft_strstrjoin(cwd, "/", str);
	free(str);
	i = 0;
	while (av[++i])
		tab[i] = ft_strdup(av[i]);
	tab[i] = NULL;
	i = is_absolute(tab, shell, pipe);
	ft_strdelpp(&tab);
	return (i);
}

char	is_absolute(char **av, t_sh *shell, char pipe)
{
	t_stat		file;

	if (**av && **av == '/')
	{
		if (!lstat(*av, &file) && S_ISREG(file.st_mode) && is_binary(*av) &&
			!access(*av, R_OK | X_OK))
		{
			run_binary(ft_strdup(*av), av, shell, pipe);
			return (1);
		}
		else if (access(*av, F_OK) == -1)
		{
			errexit(*av, "No such file or directory.");
			return (1);
		}
		else if (access(*av, R_OK | X_OK) == -1)
		{
			errexit(*av, "Permission denied.");
			return (1);
		}
	}
	else if (**av && **av == '.' && *(*av + 1) && *(*av + 1) == '/')
		return (current_binary(av, shell, pipe));
	return (0);
}
