/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/18 18:29:37 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/12/06 17:22:42 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

static t_sh	g_sh;

static void	init_shell(t_sh *shell, t_lexer *lexer)
{
	shell->op = NULL;
	shell->total_command = NULL;
	shell->state = BASIC_SHELL;
	shell->ref_pos = -1;
	shell->saved = NULL;
	shell->history_pos = -1;
	shell->history_mem = NULL;
	shell->lexer = lexer;
	shell->hash = NULL;
	shell->env = NULL;
	shell->export = NULL;
	shell->save_env = NULL;
	shell->return_value = 0;
	shell->have_write_error = 0;
	shell->toaddstr = NULL;
	shell->hist_first = 0;
	shell->ssbqt = 0;
	shell->pipe_ss = NULL;
	shell->shell_var = NULL;
	shell->read_delimiter = 0;
	shell->read_nchar = 0;
	shell->tab_request = 0;
	shell->lst_pid = NULL;
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
	if (sig == SIGCONT)
		set_our_term(&g_sh);
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
		g_sh.tab_request = 0;
		move_to(g_sh.pos.last.x, g_sh.pos.last.y);
		ft_putstr("\n");
		do_termcap("ce");
		print_prompt(BASIC_SHELL, NULL);
	}
}

int			main(int ac, char **av, char **termenv)
{
	t_lexer	lexer;

	(void)ac;
	(void)av;
	init_shell(&g_sh, &lexer);
	g_sh.env = get_env(termenv);
	g_sh.shell_var = get_env(termenv);
	g_sh.export = add_to_export(termenv);
	inc_shlvl(g_sh.env);
	inc_shlvl(g_sh.shell_var);
	init_termcap(&g_sh, g_sh.env);
	get_current_path(g_sh.env);
	signal(SIGINT, sig_hand);
	signal(SIGTERM, sig_hand);
	signal(SIGCONT, sig_hand);
	load_history(&g_sh, g_sh.env);
	shell_loop(&g_sh);
	return (0);
}
