/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_cmd_opt.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/10 20:26:36 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/09/01 16:47:22 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "tosh.h"

void		hist_d(t_sh *shell, int nb)
{
	int		i;
	char	j;
	int		count;
	char	**new;

	count = (int)ft_counttab(shell->history);
	if (!shell->history || nb <= 0 || nb > count)
	{
		errexit("history", "Bad offset.");
		return ;
	}
	if (!(new = (char**)malloc(sizeof(char*) * (count))))
		ft_exiterror("Malloc failed", 1);
	i = -1;
	j = 0;
	while (shell->history[++i])
	{
		if (i + nb == count && (j = 1))
			continue ;
		new[i - j] = ft_strdup(shell->history[i]);
	}
	new[i] = NULL;
	ft_strdelpp(&(shell->history));
	shell->history = new;
}

static void	hist_r2(t_sh *shell, char **toadd, char **new)
{
	int			i;
	int			count;

	i = ft_counttab(toadd);
	count = -1;
	while (--i >= 0)
		new[++count] = ft_strdup(toadd[i]);
	ft_strdelpp(&toadd);
	i = -1;
	while (shell->history[++i])
		new[++count] = ft_strdup(shell->history[i]);
	new[++count] = NULL;
	ft_strdelpp(&(shell->history));
	shell->history = new;
}

void		hist_r(t_sh *shell, char *path)
{
	char		**toadd;
	char		**new;
	struct stat	buff;
	int			count;

	if (!path || !*path || stat(path, &buff) == -1 || \
		!(toadd = get_history(shell, buff.st_size, path, 0)))
	{
		errexit("history", "Unable to open file.");
		return ;
	}
	count = ft_counttab(shell->history) + ft_counttab(toadd);
	if (!(new = (char**)malloc(sizeof(char*) * (count + 1))))
		ft_exiterror("Malloc failed", 1);
	hist_r2(shell, toadd, new);
}

void		hist_w(t_sh *shell, char *path)
{
	char		**new;
	struct stat	buff;

	if (!path || !*path || stat(path, &buff) == -1 || \
	!(new = get_history(shell, buff.st_size, path, 0)))
	{
		errexit("history", "Unable to open file.");
		return ;
	}
	ft_strdelpp(&(shell->history));
	shell->history = new;
}

void		hist_a(t_sh *shell, char *path)
{
	int			fd;
	int			i;
	int			max;

	i = shell->hist_first;
	if (i > (max = ft_counttab(shell->history)))
	{
		i = 0;
		return ;
	}
	if (!path || !*path || (fd = open(path, O_WRONLY | O_CREAT | \
		O_TRUNC, S_IRWXU)) == -1)
	{
		errexit("history", "Unable to open file.");
		return ;
	}
	while (++i < max)
	{
		write(fd, shell->history[max - i], ft_strlen(shell->history[max - i]));
		write(fd, "\n", 1);
	}
	close(fd);
}
