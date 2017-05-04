/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/30 00:01:43 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/05/04 18:04:27 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "21sh.h"

static void	remove_char(t_sh *shell, int current, char *tmp, char rm_first)
{
	tmp = shell->saved;
	if (rm_first)
		shell->saved = ft_strsub(tmp, 1, ft_strlen(tmp) - 1);
	else
		shell->saved = ft_strsub(tmp, 0, ft_strlen(tmp) - 1);
	free(tmp);
	ft_putchar(shell->command[current]);
}

static void	ctrl_maj_rigth(t_sh *shell, int current, char *tmp)
{
	if (shell->ref_pos <= current)
	{
		if (shell->saved)
		{
			tmp = shell->saved;
			shell->saved = ft_strjoinchar(tmp, shell->command[current]);
			free(tmp);
		}
		else
			shell->saved = ft_strjoinchar("", shell->command[current]);
		do_termcap("mr");
		do_termcap("us");
		ft_putchar(shell->command[current]);
		do_termcap("ue");
		do_termcap("me");
	}
	else
		remove_char(shell, current, tmp, 1);
	move_to(shell->pos.cursor.x, shell->pos.cursor.y);
	right_arrow(shell);
}

static void	ctrl_maj_left(t_sh *shell, int current, char *tmp, char *tmp2)
{
	left_arrow(shell);
	if (shell->ref_pos >= current)
	{
		tmp = shell->saved;
		shell->saved = ft_strjoinchar("", shell->command[current - 1]);
		if (tmp)
		{
			tmp2 = shell->saved;
			shell->saved = ft_strjoin(tmp2, tmp);
			free(tmp);
			free(tmp2);
		}
		do_termcap("mr");
		do_termcap("us");
		ft_putchar(shell->command[current - 1]);
		do_termcap("ue");
		do_termcap("me");
	}
	else
		remove_char(shell, current - 1, tmp, 0);
	move_to(shell->pos.cursor.x, shell->pos.cursor.y);
}

void		copy_data(t_sh *shell, unsigned long c, int current)
{
	current = get_current_pos_in_command(shell);
	if (shell->ref_pos == -1)
	{
		if (shell->saved)
		{
			free(shell->saved);
			shell->saved = NULL;
		}
		shell->ref_pos = get_current_pos_in_command(shell);
	}
	if (c == CTRL_MAJ_LEFT)
	{
		if (shell->pos.cursor.y == shell->pos.first.y && \
			shell->pos.cursor.x == shell->pos.first.x)
			return ;
		ctrl_maj_left(shell, current, NULL, NULL);
	}
	else if (c == CTRL_MAJ_RIGTH)
	{
		if (shell->pos.cursor.y == shell->pos.last.y && \
			shell->pos.cursor.x == shell->pos.last.x)
			return ;
		ctrl_maj_rigth(shell, current, NULL);
	}
}

void		clean_selected(t_sh *shell)
{
	int		i;
	int		current;
	t_pos	tmp;

	tmp.x = shell->pos.cursor.x;
	tmp.y = shell->pos.cursor.y;
	current = get_current_pos_in_command(shell);
	if (shell->ref_pos < current)
	{
		i = current;
		while (shell->ref_pos < i--)
			left_arrow(shell);
		while (i++ < current)
			ft_putchar(shell->command[i]);
	}
	else
	{
		i = current;
		while (shell->ref_pos > i++)
			ft_putchar(shell->command[i - 1]);
	}
	shell->pos.cursor.x = tmp.x;
	move_to(shell->pos.cursor.x, (shell->pos.cursor.y = tmp.y));
	shell->ref_pos = -1;
}
