/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/18 00:51:22 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/03/26 04:28:45 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <unistd.h>
#include "21sh.h"

static void	clear_line(t_sh *shell)
{
	int		i;
	char	white_space[shell->j + 2];

	i = -1;
	while (++i < shell->j + 1)
		white_space[i] = ' ';
	white_space[shell->j + 1] = '\0';
	move_to(shell->pos.first.x, shell->pos.first.y);
	ft_putstr(white_space);
	move_to(shell->pos.first.x, shell->pos.first.y);
	ft_bzero(shell->command, shell->j);
	shell->j = -1;
	get_cursor(shell);
	shell->pos.first = (t_pos){shell->pos.cursor.x, shell->pos.cursor.y};
	shell->pos.last = (t_pos){shell->pos.cursor.x, shell->pos.cursor.y};
}

void	browse_history(t_sh *shell, char arrow)
{
	int			i;
	int			max;

	if (!shell->history)
		return ;
	max = -1;
	while (shell->history[++max])
		;
	--max;
	i = -1;
	if (arrow == 'A' && shell->history_pos < max)
	{
		if (shell->j > -1)
		{
			if (!shell->history_mem && shell->history_pos == -1)
			{
				shell->command[++(shell->j)] = '\0';
				shell->history_mem = ft_strdup(shell->command);
			}
			clear_line(shell);
		}
		++(shell->history_pos);
		while (shell->history[shell->history_pos][++i])
		{
			if (add_char(shell->command, &(shell->j), shell, shell->history[shell->history_pos][i]))
			{
				sig_hand(0);
				return ;
			}
		}
	}
	else if (arrow == 'B' && shell->history_pos <= max && shell->history_pos > 0) 
	{
		if (shell->j > -1)
			clear_line(shell);
		--(shell->history_pos);
		while (shell->history[shell->history_pos][++i])
		{
			if (add_char(shell->command, &(shell->j), shell, shell->history[shell->history_pos][i]))
			{
				sig_hand(0);
				return ;
			}
		}
	}
	else if (arrow == 'B' && !shell->history_pos && shell->j > -1)
	{
		shell->history_pos = -1;
		clear_line(shell);
		if (shell->history_mem)
		{
			while (shell->history_mem[++i])
			{
				if (add_char(shell->command, &(shell->j), shell, shell->history_mem[i]))
				{
					sig_hand(0);
					return ;
				}
			}
			free(shell->history_mem);
			shell->history_mem = NULL;
		}
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

	shell->history_pos = -1;
	shell->history_mem = NULL;
	if (!(home = find_env(env, "HOME")))
		return ;
	home = ft_strjoin(home, "/.21sh_history");
	if (!stat(home, &buff))
		get_history(shell, buff.st_size, home);
	else
		shell->history = NULL;
	free(home);
}