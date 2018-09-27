/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_line.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/21 21:14:03 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/09/27 17:45:17 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include "tosh.h"

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

static char	get_line2(t_sh *shell, unsigned long buff, enum e_state *state)
{
	if (shell->saved && shell->ref_pos > -1 && buff != CTRL_MAJ_LEFT && \
		buff != CTRL_MAJ_RIGTH)
		clean_selected(shell);
	if (shell->tab_request && (char)buff != 'y' && (char)buff != 'n')
		;
	else if ((buff == TAB || shell->tab_request) && *state != READ_CMD)
		go_completion(shell, (char)buff);
	else if (buff == EOT && shell->j == -1)
		ctrl_d(shell);
	else if (buff == UP_ARROW || buff == DOWN_ARROW || buff == LEFT_ARROW || \
		buff == RIGHT_ARROW || buff == HOME || buff == END)
		arrows(shell, buff);
	else if (buff == CTRL_P && shell->saved && *state != READ_CMD)
		past_data(shell);
	else if (buff == MAJ_LEFT || buff == MAJ_RIGTH || buff == MAJ_UP || \
		buff == MAJ_DOWN || buff == CTRL_MAJ_LEFT || buff == CTRL_MAJ_RIGTH)
		maj_arrows(shell, buff);
	else if (buff == DELETE && shell->j > -1 && !(shell->pos.cursor.x == \
		shell->pos.first.x && shell->pos.cursor.y == shell->pos.first.y))
		delete_char(shell->command, &(shell->j), shell);
	else if (buff == CTRL_R && *state != READ_CMD)
		search_mode(shell);
	else
		return (0);
	return (1);
}

void		add_all_char(t_sh *shell)
{
	int	i;

	i = -1;
	while (shell->toaddstr[++i])
		add_char(shell->command, &(shell->j), shell, shell->toaddstr[i]);
	ft_strdel(&(shell->toaddstr));
}

char		*get_line(t_sh *shell, unsigned long buff, enum e_state *state, \
				char *op)
{
	print_prompt(*state, op);
	shell->j = -1;
	if (shell->toaddstr)
		add_all_char(shell);
	while (1)
	{
		buff = 0;
		read(0, &buff, sizeof(unsigned long));
		// add_char(shell->command, &(shell->j), shell, '.');
		// add_char(shell->command, &(shell->j), shell, ';');
		// add_char(shell->command, &(shell->j), shell, 'c');
		// buff = TAB;
		if (get_line2(shell, buff, state))
			;
		else if (buff == ENTER || (*state == READ_CMD && (buff == \
			(unsigned long)shell->read_delimiter || !(shell->read_nchar - 1))))
		{
			move_to(shell->pos.last.x, shell->pos.last.y);
			break ;
		}
		else if ((char)buff >= 32 && (char)buff <= 126)
			if (add_char(shell->command, &(shell->j), shell, buff))
				return (ft_strdup(""));
		if (shell->read_nchar > 0)
			--(shell->read_nchar);
	}
	shell->command[++(shell->j)] = '\0';
	return (ft_strdup(shell->command));
}
