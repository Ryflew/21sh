/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_termcap.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/20 02:29:29 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/07/11 01:10:33 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <curses.h>
#include <term.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include "tosh.h"

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
	{
		errexit("21sh", "Impossible de set le nouveau terminal");
		exit(EXIT_FAILURE);
	}
	ft_memmove(&shell->our, &term, sizeof(term));
}

void		init_termcap(t_sh *shell, t_env *env)
{
	struct winsize	w;
	t_termios		tmp;
	char			*term_name;

	if ((term_name = find_env(env, "TERM")) == NULL)
	{
		ft_fputstr("Nom de terminal introuvable dans l'environnement.\n", 2);
		exit(EXIT_FAILURE);
	}
	if (tgetent(0, term_name) != 1)
	{
		ft_fputstr("Impossible de récupérer la structure termios.\n", 2);
		exit(EXIT_FAILURE);
	}
	tcgetattr(0, &tmp);
	ft_memmove(&shell->old, &tmp, sizeof(tmp));
	build_term(shell);
	ioctl(0, TIOCGWINSZ, &w);
	shell->pos.max_window.x = w.ws_col;
	shell->pos.max_window.y = w.ws_row - 1;
}

void		set_old_term(t_sh *shell, char *str)
{
	if (ft_strstr(str, "21sh"))
		signal(SIGINT, SIG_IGN);
	if (tcsetattr(0, TCSADRAIN, &(shell->old)) == -1)
	{
		errexit("21sh", "Impossible de set l'ancien terminal");
		exit(EXIT_FAILURE);
	}
}

void		set_our_term(t_sh *shell)
{
	signal(SIGINT, sig_hand);
	if (tcsetattr(0, TCSADRAIN, &(shell->our)) == -1)
	{
		errexit("21sh", "Impossible de set le nouveau terminal");
		exit(EXIT_FAILURE);
	}
}
