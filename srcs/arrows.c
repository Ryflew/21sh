/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arrows.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/22 23:06:06 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/08/10 03:07:19 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

void	left_arrow(t_sh *shell)
{
	if (shell->pos.first.y == shell->pos.cursor.y && \
		shell->pos.cursor.x > shell->pos.first.x)
		move_to(--(shell->pos.cursor.x), shell->pos.cursor.y);
	else if (shell->pos.first.y < shell->pos.cursor.y)
	{
		if (shell->pos.cursor.x > 0)
			move_to(--(shell->pos.cursor.x), shell->pos.cursor.y);
		else if (!shell->pos.cursor.x)
			move_to((shell->pos.cursor.x = shell->pos.max_window.x - 1), \
				--(shell->pos.cursor.y));
	}
}

void	right_arrow(t_sh *shell)
{
	if (!(shell->pos.cursor.x == shell->pos.last.x && \
		shell->pos.cursor.y == shell->pos.last.y))
	{
		if (shell->pos.cursor.x == shell->pos.max_window.x - 1)
			move_to((shell->pos.cursor.x = 0), ++(shell->pos.cursor.y));
		else if (shell->pos.cursor.x < shell->pos.max_window.x)
			move_to(++(shell->pos.cursor.x), shell->pos.cursor.y);
	}
}

void	past_data(t_sh *shell)
{
	int		i;
	int		max;

	max = ft_strlen(shell->saved);
	i = 0;
	while (i < max)
		if (add_char(shell->command, &(shell->j), shell, shell->saved[i++]))
			return ;
}
