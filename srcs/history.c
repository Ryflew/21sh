/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/18 00:51:22 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/06/15 18:26:07 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "21sh.h"

void	clear_line(t_sh *shell)
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
	if (shell->j > -1)
		ft_bzero(shell->command, shell->j);
	shell->j = -1;
	shell->pos.last = (t_pos){shell->pos.first.x, shell->pos.first.y};
	shell->pos.cursor = shell->pos.first;
}

static char	browse_history_top(t_sh *shell, int *i)
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
	while (shell->history[shell->history_pos][++(*i)])
	{
		if (add_char(shell->command, &(shell->j), shell, \
			shell->history[shell->history_pos][*i]))
		{
			sig_hand(0);
			return (1);
		}
	}
	return (0);
}

static char	browse_history_bot_last(t_sh *shell, int *i)
{
	shell->history_pos = -1;
	clear_line(shell);
	if (shell->history_mem)
	{
		while (shell->history_mem[++(*i)])
		{
			if (add_char(shell->command, &(shell->j), shell, \
				shell->history_mem[*i]))
			{
				sig_hand(0);
				return (1);
			}
		}
		free(shell->history_mem);
		shell->history_mem = NULL;
	}
	return (0);
}

void	browse_history(t_sh *shell, unsigned long arrow)
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
	if (arrow == UP_ARROW && shell->history_pos < max)
	{
		if (browse_history_top(shell, &i))
			return ;
	}
	else if (arrow == DOWN_ARROW && shell->history_pos <= max && \
		shell->history_pos > 0)
	{
		if (shell->j > -1)
			clear_line(shell);
		--(shell->history_pos);
		while (shell->history[shell->history_pos][++i])
		{
			if (add_char(shell->command, &(shell->j), shell, \
				shell->history[shell->history_pos][i]))
			{
				sig_hand(0);
				return ;
			}
		}
	}
	else if (arrow == DOWN_ARROW && !shell->history_pos && shell->j > -1)
		if (browse_history_bot_last(shell, &i))
			return ;
}
