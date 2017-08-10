/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_line.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/21 21:14:03 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/08/10 03:06:10 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include "tosh.h"

static void	arrows(t_sh *shell, unsigned long c)
{
	if (c == UP_ARROW)
		browse_history(shell, c);
	else if (c == DOWN_ARROW)
		browse_history(shell, c);
	else if (c == RIGHT_ARROW)
		right_arrow(shell);
	else if (c == LEFT_ARROW)
		left_arrow(shell);
	else if (c == HOME)
		move_to((shell->pos.cursor.x = shell->pos.first.x), \
			(shell->pos.cursor.y = shell->pos.first.y));
	else if (c == END)
		move_to((shell->pos.cursor.x = shell->pos.last.x), \
			(shell->pos.cursor.y = shell->pos.last.y));
}

static void	maj_arrows(t_sh *shell, unsigned long c)
{
	if (c == MAJ_LEFT)
		maj_left_arrow(shell);
	else if (c == MAJ_RIGTH)
		maj_right_arrow(shell);
	else if (c == MAJ_UP)
		maj_up_arrow(shell);
	else if (c == MAJ_DOWN)
		maj_down_arrow(shell);
	else if (c == CTRL_MAJ_LEFT || c == CTRL_MAJ_RIGTH)
		copy_data(shell, c, 0);
}

static void	ctrl_d(t_sh *shell)
{
	if (shell->state == BASIC_SHELL)
	{
		save_history(shell);
		tcsetattr(0, TCSADRAIN, &(shell->old));
		ft_putstr("exit\n");
		exit(EXIT_SUCCESS);
	}
	else
		sig_hand(0);
}

static char	get_line2(t_sh *shell, unsigned long buff)
{
	if (shell->saved && shell->ref_pos > -1 && buff != CTRL_MAJ_LEFT && \
		buff != CTRL_MAJ_RIGTH)
		clean_selected(shell);
	if (buff == EOT && shell->j == -1)
		ctrl_d(shell);
	else if (buff == UP_ARROW || buff == DOWN_ARROW || buff == LEFT_ARROW || \
		buff == RIGHT_ARROW || buff == HOME || buff == END)
		arrows(shell, buff);
	else if (buff == CTRL_P && shell->saved)
		past_data(shell);
	else if (buff == MAJ_LEFT || buff == MAJ_RIGTH || buff == MAJ_UP || \
		buff == MAJ_DOWN || buff == CTRL_MAJ_LEFT || buff == CTRL_MAJ_RIGTH)
		maj_arrows(shell, buff);
	else if (buff == DELETE && shell->j > -1 && !(shell->pos.cursor.x == \
		shell->pos.first.x && shell->pos.cursor.y == shell->pos.first.y))
		delete_char(shell->command, &(shell->j), shell);
	else if (buff == CTRL_R)
		search_mode(shell);
	else if (buff == TAB && shell->j != -1)
		go_completion(shell);
	else
		return (0);
	return (1);
}

char		*get_line(t_sh *shell, unsigned long buff, e_state *state, char *op)
{
	print_prompt(*state, op);
	shell->have_write_error = 0;
	shell->j = -1;
	// return (ft_strdup("env -i env"));
	while (1)
	{
		buff = 0;
		read(0, &buff, sizeof(unsigned long));
		if (get_line2(shell, buff))
			;
		else if (buff == ENTER)
		{
			move_to(shell->pos.last.x, shell->pos.last.y);
			break ;
		}
		else if (buff >= 32 && buff <= 126)
			if (add_char(shell->command, &(shell->j), shell, (char)buff))
				return (ft_strdup(""));
	}
	shell->command[++(shell->j)] = '\0';
	return (ft_strdup(shell->command));
}
