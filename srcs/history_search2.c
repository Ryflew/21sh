/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_search2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/10 20:27:29 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/09/02 16:47:15 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

void	clear_to_bot(t_sh *shell)
{
	int		i;

	i = shell->pos.last.y;
	move_to(0, i + 1);
	while (++i <= shell->pos.max_window.y)
		do_termcap("dl");
}

int		check_hist_occur(t_sh *shell, char *str)
{
	int		len;

	len = ft_counttab(shell->history);
	while (len > 0)
	{
		if (ft_strcont(shell->history[--len], str))
			return (len);
	}
	return (-1);
}

void	print_history2(t_sh *shell, t_sh *bis_sh)
{
	if (shell->pos.last.y == shell->pos.max_window.y)
	{
		bis_sh->pos.first = (t_pos){16, shell->pos.last.y};
		bis_sh->pos.last = (t_pos){16, shell->pos.last.y};
		bis_sh->pos.cursor = (t_pos){16, shell->pos.last.y};
		--(shell->pos.last.y);
		--(shell->pos.cursor.y);
		--(shell->pos.first.y);
	}
	else
	{
		bis_sh->pos.first = (t_pos){16, shell->pos.last.y + 1};
		bis_sh->pos.last = (t_pos){16, shell->pos.last.y + 1};
		bis_sh->pos.cursor = (t_pos){16, shell->pos.last.y + 1};
		move_to(shell->pos.last.x, shell->pos.last.y);
	}
}

void	init_bis_sh(t_sh *shell, t_sh *bis_sh)
{
	bis_sh->j = -1;
	bis_sh->pos = shell->pos;
	bis_sh->pos.cursor.x = 16;
	bis_sh->pos.first.x = 16;
	bis_sh->pos.last.x = 16;
}
