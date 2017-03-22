/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/18 18:29:37 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/03/22 02:42:51 by vdarmaya         ###   ########.fr       */
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

static void	print_prompt(void)
{
	ft_putstr("[21sh:");
	ft_putstr(g_sh.prompt);
	ft_putstr("] $> ");
	get_cursor(&g_sh);
}

// static void	sig_hand(int sig)
// {
// 	if (!stop_binary(sig))
// 	{
// 		ft_putstr("\n");
// 		print_prompt();
// 	}
// }

int			main(int ac, char **av, char **termenv)
{
	char	buff[3];
	char	*command;
	t_env	*env;

	(void)ac;
	(void)av;
	env = get_env(termenv);
	init_termcap(&g_sh, env);
	get_current_path(env);
	print_prompt();
	// signal(SIGINT, sig_hand);
	// load_history(&g_sh, env);
	while (1)
	{
		command = get_line(&g_sh, buff);
		if (*command)
			go_core(command, &env, &g_sh);
		free(command);
		ft_putchar('\n');
		print_prompt();
	}
	return (0);
}
