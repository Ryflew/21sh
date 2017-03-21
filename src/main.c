/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/18 18:29:37 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/03/21 05:59:28 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "get_next_line.h"
#include "21sh.h"

static t_sh	g_sh;

static void	get_current_path(t_env *env)
{
	char	*cwd;
	char	buff[4097];

	cwd = getcwd(buff, 4097);
	g_sh.prompt = get_with_tilde(cwd, env);
}

static void	print_prompt(char *path)
{
	ft_putstr("[21sh:");
	ft_putstr(path);
	ft_putstr("] $> ");
}

static void	sig_hand(int sig)
{
	if (!stop_binary(sig))
	{
		// move_to((x -= 2), y);
		// do_termcap("ce")
		// move_to(x, y);
		ft_putstr("\n");
		print_prompt(g_sh.prompt);
	}
}

int			main(int ac, char **av, char **termenv)
{
	char	*command;
	t_env	*env;

	(void)ac;
	(void)av;
	env = get_env(termenv);
	// init_termcap(&g_sh, env);
	get_current_path(env);
	print_prompt(g_sh.prompt);
	signal(SIGINT, sig_hand);
	// load_history(&g_sh, env);
	while (get_next_line(0, &command))
	{
		if (*command)
			go_core(command, &env, &g_sh);
		free(command);
		print_prompt(g_sh.prompt);
	}
	return (0);
}
