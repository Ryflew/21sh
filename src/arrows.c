/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arrows.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/22 23:06:06 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/03/26 01:36:33 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

void	left_arrow(t_sh *shell)
{
	if (shell->pos.first.y == shell->pos.cursor.y && shell->pos.cursor.x > shell->pos.first.x)
		move_to(--(shell->pos.cursor.x), shell->pos.cursor.y);
	else if (shell->pos.first.y < shell->pos.cursor.y)
	{
		if (shell->pos.cursor.x > 0)
			move_to(--(shell->pos.cursor.x), shell->pos.cursor.y);
		else if (!shell->pos.cursor.x)
			move_to((shell->pos.cursor.x = shell->pos.max_window.x - 1), --(shell->pos.cursor.y));
	}
}

void	rigth_arrow(t_sh *shell)
{
	if (!(shell->pos.cursor.x == shell->pos.last.x && shell->pos.cursor.y == shell->pos.last.y))
	{
		if (shell->pos.cursor.x == shell->pos.max_window.x - 1)
			move_to((shell->pos.cursor.x = 0), ++(shell->pos.cursor.y));
		else if (shell->pos.cursor.x < shell->pos.max_window.x)
			move_to(++(shell->pos.cursor.x), shell->pos.cursor.y);
	}
}
