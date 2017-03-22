/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_termcap.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/20 02:29:29 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/03/22 04:51:11 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <curses.h>
#include <term.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include "21sh.h"

static void	build_term(t_sh *shell)
{
	t_termios	term;

	term = shell->old;
	term.c_lflag &= ~(ICANON);
	term.c_lflag &= ~(ECHO);
	term.c_cflag &= ~CREAD;
	term.c_cc[VMIN] = 1;
	term.c_cc[VTIME] = 0;
	if (tcsetattr(0, TCSADRAIN, &term) == -1)
		errexit("21sh", "Impossible de set le nouveau terminal");
}

void	init_termcap(t_sh *shell, t_env *env)
{
	struct winsize	w;	
	t_termios		tmp;
	char			*term_name;

	if ((term_name = find_env(env, "TERM")) == NULL)
	{
		ft_putstr_fd("Nom de terminal introuvable dans l'environnement.\n", 2);
		exit(EXIT_FAILURE);
	}
	if (tgetent(0, term_name) != 1)
	{
		ft_putstr_fd("Impossible de récupérer la structure termios.\n", 2);
		exit(EXIT_FAILURE);
	}
	tcgetattr(0, &tmp);
	ft_memmove(&shell->old, &tmp, sizeof(tmp));
	build_term(shell);
	ioctl(0, TIOCGWINSZ, &w);
	shell->max_window.x = w.ws_col;
	shell->max_window.y = w.ws_row - 1;
}
