/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_line.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/21 21:14:03 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/03/24 04:02:12 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include "21sh.h"

static void	arrows(char *buff, char *command, int *j, t_sh *shell)
{
	(void)command;
	(void)j;
	if (buff[2] == 'A') // haut
		;
	else if (buff[2] == 'B') // bas
		;
	else if (buff[2] == 'C') // droite
		rigth_arrow(shell);
	else if (buff[2] == 'D') // gauche
		left_arrow(shell);
}

static void	delete_char(char *command, int *j, t_sh *shell)
{
	command[(*j)--] = '\0';
	if (shell->pos.cursor.x > 0)
	{
		--(shell->pos.last.x);
		move_to(--(shell->pos.cursor.x), shell->pos.cursor.y);
	}
	else
	{
		--(shell->pos.last.y);
		shell->pos.last.x = shell->pos.max_window.x - 1;
		move_to((shell->pos.cursor.x = shell->pos.max_window.x - 1), --(shell->pos.cursor.y));
	}
	ft_putchar(' ');
	move_to(shell->pos.cursor.x, shell->pos.cursor.y);

}

static char	*ctrl_d(t_sh *shell, char is_bracket)
{
	if (!is_bracket)
	{
		tcsetattr(0, TCSADRAIN, &(shell->old));
		ft_putstr("exit\n");
		exit(EXIT_SUCCESS);
	}
	else
		return (ft_strdup("\t"));
}
#include <unistd.h>
static void	add_char(char *command, int *j, t_sh *shell, char c)
{
	int		total;
	int		i;

	if (shell->pos.cursor.y == shell->pos.last.y && shell->pos.cursor.x == shell->pos.last.x)
	{
		command[++(*j)] = c;
		ft_putchar(c);
		if (++(shell->pos.cursor.x) == shell->pos.max_window.x)
		{
			ft_putchar(' ');
			if (shell->pos.cursor.y < shell->pos.max_window.y)
				move_to((shell->pos.cursor.x = 0), ++(shell->pos.cursor.y));
			else
			{
				--(shell->pos.first.y);
				move_to((shell->pos.cursor.x = 0), shell->pos.cursor.y);
			}
		}
		shell->pos.last.x = shell->pos.cursor.x;
		shell->pos.last.y = shell->pos.cursor.y;
	}
	else
	{
		if (shell->pos.first.y == shell->pos.cursor.y)
			total = shell->pos.cursor.x - shell->pos.first.x;
		else
			total = (shell->pos.max_window.x - shell->pos.first.x) + \
				((shell->pos.cursor.y - shell->pos.first.y - 1) * shell->pos.max_window.x) + shell->pos.cursor.x;
		i = ++(*j) + 1;
		while (total < --i)
			command[i] = command[i - 1];
		command[total] = c;
		while (total <= *j)
			ft_putchar(command[total++]);
		if (++(shell->pos.cursor.x) == shell->pos.max_window.x)
		{
			shell->pos.cursor.x = 0;
			++(shell->pos.cursor.y);
		}
		if (++(shell->pos.last.x) == shell->pos.max_window.x)
		{
			ft_putchar(' ');
			shell->pos.last.x = 0;
			if (shell->pos.last.y < shell->pos.max_window.y)
				++(shell->pos.last.y);
			else if (shell->pos.last.y == shell->pos.max_window.y)
			{
				--(shell->pos.cursor.y);
				--(shell->pos.first.y);
			}
		}
		move_to(shell->pos.cursor.x, shell->pos.cursor.y);
	}
}

char	*get_line(t_sh *shell, char *buff, char is_bracket)
{
	int			j;
	char		command[ARG_MAX];

	shell->pos.first = (t_pos){shell->pos.cursor.x, shell->pos.cursor.y};
	shell->pos.last = (t_pos){shell->pos.cursor.x, shell->pos.cursor.y};
	j = -1;
	while (1)
	{
		if (j + 1 == ARG_MAX)
		{
			ft_putstr("Input too long.");
			return (ft_strdup(""));
		}
		ft_bzero(buff, 3);
		read(0, buff, 3);
		if (buff[0] == 4 && !buff[1] && !buff[2])
			return (ctrl_d(shell, is_bracket));
		else if (buff[0] == 27 && buff[1] == 91)
			arrows(buff, command, &j, shell);
		else if (buff[0] == 127 && !buff[1] && !buff[2] && j > -1)
			delete_char(command, &j, shell);
		else if (buff[0] == 10 && !buff[1] && !buff[2])
		{
			move_to(shell->pos.last.x, shell->pos.last.y);
			break ;
		}
		else if (ft_isprint(buff[0]) && !buff[1] && !buff[2])
			add_char(command, &j, shell, buff[0]);
	}
	command[++j] = '\0';
	return (ft_strdup(command));
}
