/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete_char.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/28 00:15:57 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/04/02 02:41:09 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

static void	remove_last(char *command, int *j, t_sh *shell)
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
		move_to((shell->pos.cursor.x = shell->pos.max_window.x - 1), \
			--(shell->pos.cursor.y));
	}
	ft_putchar(' ');
	move_to(shell->pos.cursor.x, shell->pos.cursor.y);
}

static void	remove_middle(t_sh *shell)
{
	if (--(shell->pos.cursor.x) < 0)
	{
		shell->pos.cursor.x = shell->pos.max_window.x - 1;
		--(shell->pos.cursor.y);
	}
	if (--(shell->pos.last.x) < 0)
	{
		shell->pos.last.x = shell->pos.max_window.x - 1;
		--(shell->pos.last.y);
	}
	move_to(shell->pos.cursor.x, shell->pos.cursor.y);
}

void		delete_char(char *command, int *j, t_sh *shell)
{
	int		total;
	int		i;

	if (shell->pos.cursor.y == shell->pos.last.y && shell->pos.cursor.x == \
		shell->pos.last.x)
		remove_last(command, j, shell);
	else
	{
		total = get_current_pos_in_command(shell);
		i = --total;
		while (++i <= *j)
			command[i - 1] = command[i];
		remove_middle(shell);
		while (total < *j)
			ft_putchar(command[total++]);
		ft_putchar(' ');
		move_to(shell->pos.cursor.x, shell->pos.cursor.y);
		--(*j);
	}
}
