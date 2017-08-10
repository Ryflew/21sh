/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maj_arrows.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/29 20:51:35 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/08/10 03:06:32 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

void	maj_up_arrow(t_sh *shell)
{
	if (shell->pos.cursor.y == shell->pos.first.y)
		return ;
	if (shell->pos.cursor.y - 1 == shell->pos.first.y && \
		shell->pos.cursor.x < shell->pos.first.x)
		move_to((shell->pos.cursor.x = shell->pos.first.x), \
			(shell->pos.cursor.y = shell->pos.first.y));
	else
		move_to(shell->pos.cursor.x, --(shell->pos.cursor.y));
}

void	maj_down_arrow(t_sh *shell)
{
	if (shell->pos.cursor.y == shell->pos.last.y)
		return ;
	if (shell->pos.cursor.y + 1 == shell->pos.last.y && \
		shell->pos.cursor.x > shell->pos.last.x)
		move_to((shell->pos.cursor.x = shell->pos.last.x), \
			(shell->pos.cursor.y = shell->pos.last.y));
	else
		move_to(shell->pos.cursor.x, ++(shell->pos.cursor.y));
}

void	maj_left_arrow(t_sh *shell)
{
	int		i;
	char	is_space;

	is_space = 0;
	i = get_current_pos_in_command(shell);
	if (shell->pos.cursor.y == shell->pos.last.y && \
		shell->pos.cursor.x == shell->pos.last.x)
		is_space = 1;
	while (i > 0)
	{
		if (!is_space && shell->command[i] == ' ')
		{
			while (i > 0 && shell->command[i] == ' ')
			{
				left_arrow(shell);
				--i;
			}
			is_space = 1;
		}
		if (is_space && shell->command[i - 1] == ' ')
			break ;
		left_arrow(shell);
		--i;
	}
}

void	maj_right_arrow(t_sh *shell)
{
	int		i;

	i = get_current_pos_in_command(shell);
	while (i <= shell->j)
	{
		if (shell->command[i] == ' ')
		{
			while (i < shell->j && shell->command[i] == ' ')
			{
				right_arrow(shell);
				++i;
			}
			break ;
		}
		right_arrow(shell);
		++i;
	}
}
