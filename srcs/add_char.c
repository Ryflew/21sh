/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_char.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/27 22:35:49 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/09/02 17:13:58 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

static void	add_to_last(char *command, int *j, t_sh *shell, char c)
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

static void	add_to_middle(t_sh *shell)
{
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

char		add_char(char *command, int *j, t_sh *shell, unsigned long c)
{
	int		total;
	int		i;

	if (*j + 1 == MAX_CMD)
	{
		ft_putstr("\nInput too long.");
		sig_hand(0);
		return (1);
	}
	if (shell->pos.cursor.y == shell->pos.last.y && \
		shell->pos.cursor.x == shell->pos.last.x)
		add_to_last(command, j, shell, (char)c);
	else
	{
		total = get_current_pos_in_command(shell);
		i = ++(*j) + 1;
		while (total < --i)
			command[i] = command[i - 1];
		command[total] = (char)c;
		while (total <= *j)
			ft_putchar(command[total++]);
		add_to_middle(shell);
	}
	(c >> 8) ? add_char(command, j, shell, c >> 8) : 0;
	return (0);
}
