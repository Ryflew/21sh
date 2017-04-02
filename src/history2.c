/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/27 20:56:37 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/04/01 21:11:46 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "21sh.h"

void	add_line(t_sh *shell, char *toadd)
{
	int		i;
	char	**new;

	if (!shell->history)
	{
		if (!(shell->history = (char**)malloc(sizeof(char*) * 2)))
			errexit("\n21sh", "Malloc failed.\n");
		shell->history[0] = ft_strdup(toadd);
		shell->history[1] = NULL;
		return ;
	}
	if (!(new = (char**)malloc(sizeof(char*) * (ft_counttab(shell->history) + 2))))
		errexit("\n21sh", "Malloc failed.\n");
	i = -1;
	new[0] = ft_strdup(toadd);
	while (shell->history[++i])
		new[i + 1] = ft_strdup(shell->history[i]);
	new[i + 1] = NULL;
	ft_strdelpp(&(shell->history));
	shell->history = new;
}

static char **convert_history(char *str, int count)
{
	int		i;
	int		j;
	int		last;
	char	**out;

	if (!(out = (char**)malloc(sizeof(char*) * (count + 1))))
		return (NULL);
	last = 0;
	i = -1;
	j = -1;
	while (str[++i])
	{
		if (str[i] == '\n')
		{
			out[++j] = ft_strsub(str, last, i - last);
			last = i + 1;
		}
	}
	out[++j] = NULL;
	return (out);
}

static void	get_history(t_sh *shell, off_t size, char *home)
{
	int		i;
	int		fd;
	int		count;
	char	history[size + 1];

	if (!(fd = open(home, O_RDONLY)))
		return ;
	if (read(fd, history, size) == -1)
	{
		close(fd);
		return ;
	}
	close(fd);
	history[size] = '\0';
	i = -1;
	count = 0;
	while (history[++i])
		if (history[i] == '\n')
			++count;
	shell->history = convert_history(history, count);
}

void	load_history(t_sh *shell, t_env *env)
{
	char		*home;
	struct stat	buff;

	if (!(home = find_env(env, "HOME")))
		return ;
	home = ft_strjoin(home, "/.21sh_history");
	if (!stat(home, &buff))
		get_history(shell, buff.st_size, home);
	else
		shell->history = NULL;
	free(home);
}

void	save_history(t_sh *shell)
{
	int		i;
	int		fd;
	char	*home;

	if (!shell->history || !(home = find_env(shell->env, "HOME")))
		return ;
	home = ft_strjoin(home, "/.21sh_history");
	if ((fd = open(home, O_WRONLY | O_CREAT, S_IRWXU)) != -1)
	{
		i = -1;
		while (shell->history[++i])
		{
			write(fd, shell->history[i], ft_strlen(shell->history[i]));
			write(fd, "\n", 1);
		}
		close(fd);
	}
	free(home);
}
