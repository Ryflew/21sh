/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_line.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/21 21:14:03 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/03/22 05:16:30 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include "21sh.h"

static void	arrows(char *buff)
{
	if (buff[2] == 'A') // haut
		;
	else if (buff[2] == 'B') // bas
		;
	else if (buff[2] == 'C') // droite
		;
	else if (buff[2] == 'D') // gauche
		;
}

static void	delete_char(char *command, int *j, t_sh *shell)
{
	command[(*j)--] = '\0';
	if (shell->cursor.x > 0)
		move_to(--(shell->cursor.x), shell->cursor.y);
	else
		move_to((shell->cursor.x = shell->max_window.x - 1), --(shell->cursor.y));
	ft_putchar(' ');
	move_to(shell->cursor.x, shell->cursor.y);

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

char	*get_line(t_sh *shell, char *buff, char is_bracket)
{
	int		j;
	char	command[ARG_MAX];

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
			arrows(buff);
		else if (buff[0] == 127 && !buff[1] && !buff[2] && j > -1)
			delete_char(command, &j, shell);
		else if (buff[0] == 10 && !buff[1] && !buff[2])
			break ;
		else if (ft_isprint(buff[0]) && !buff[1] && !buff[2])
		{
			command[++j] = buff[0];
			ft_putchar(buff[0]);
			if (++(shell->cursor.x) == shell->max_window.x && !(shell->cursor.x = 0))
			{
				if (shell->cursor.y < shell->max_window.y)
					move_to(shell->cursor.x, ++(shell->cursor.y));
				else
				{
					ft_putchar(' ');
					move_to(shell->cursor.x, shell->cursor.y);
				}
			}

		}
	}
	command[++j] = '\0';
	return (ft_strdup(command));
}