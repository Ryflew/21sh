/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/18 00:51:22 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/03/21 23:13:40 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <unistd.h>
#include "21sh.h"

static int	init_pos_history(char **str)
{
	int		i;

	i = -1;
	while (str[++i])
		;
	return (i - 1);
}

void	browse_history(t_sh *shell, char arrow)
{
	static int	pos = -1;
	int			max;
	int			i;

	if (!shell->history)
		return ;
	if (pos == -1)
		pos = init_pos_history(shell->history);
	i = -1;
	while (shell->history[++i]) ;
	max = i - 1;
	if (!arrow && pos > 0) // up
		; // print shell->history[--pos];
	else if (arrow == 1) // down
	{
		if (pos == max)
			; // mettre une ligne vide !
		else
			; // print shell->history[++pos];
	}
}

void	add_history(char **str, t_env *env)
{
	int		i;
	int		fd;

	char	*home;

	if (!str || !(home = find_env(env, "HOME")))
		return ;
	home = ft_strjoin(home, "/.21sh_history");
	if ((fd = open(home, O_WRONLY | O_CREAT)) != -1)
	{
		i = -1;
		while (str[++i])
		{
			write(fd, str[i], ft_strlen(str[i]));
			write(fd, "\n", 1);
		}
	}
	close(fd);
	free(home);
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