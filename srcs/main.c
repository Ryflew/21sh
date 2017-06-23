/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/18 18:29:37 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/06/20 00:55:23 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

static t_sh	g_sh;

static void	init_shell(t_sh *shell, t_lexer *lexer)
{
	shell->ref_pos = -1;
	shell->saved = NULL;
	shell->history_pos = -1;
	shell->history_mem = NULL;
	lexer->string_operator = 0;
	lexer->red = 0;
	shell->lexer = lexer;
	shell->FD_IN = -1;
	shell->FD_OUT = -1;
	shell->FD_PIPE = -1;
	shell->hash = NULL;
	shell->save_env = NULL;
}

static void	get_current_path(t_env *env)
{
	char	*cwd;
	char	buff[4097];

	cwd = getcwd(buff, 4097);
	g_sh.prompt = get_with_tilde(cwd, env);
}

t_sh		*get_shell(void)
{
	return (&g_sh);
}

void		sig_hand(int sig)
{
	if (!stop_binary(sig))
	{
		if (g_sh.j > -1)
			ft_bzero(g_sh.command, g_sh.j);
		g_sh.j = -1;
		if (g_sh.op)
			free(g_sh.op);
		g_sh.op = NULL;
		if (g_sh.total_command)
			free(g_sh.total_command);
		g_sh.total_command = NULL;
		g_sh.state = BASIC_SHELL;
		move_to(g_sh.pos.last.x, g_sh.pos.last.y);
		ft_putstr("\n");
		print_prompt(BASIC_SHELL, NULL);
	}
}

int			main(int ac, char **av, char **termenv)
{
	t_lexer	lexer;

	(void)ac;
	(void)av;
	g_sh.env = get_env(termenv);
	init_shell(&g_sh, &lexer);
	init_termcap(&g_sh, g_sh.env);
	get_current_path(g_sh.env);
	signal(SIGINT, sig_hand);
	load_history(&g_sh, g_sh.env);
	shell_loop(&g_sh);
	return (0);
}
