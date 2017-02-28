/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/18 18:29:37 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/02/18 23:06:03 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "get_next_line.h"
#include "21sh.h"

static char	*g_path;

static void	get_current_path(t_env *env)
{
	char	*cwd;
	char	buff[4097];

	cwd = getcwd(buff, 4097);
	g_path = get_with_tilde(cwd, env);
}

static void	print_prompt(char *path)
{
	ft_putstr("[21sh:");
	ft_putstr(path);
	ft_putstr("] $> ");
}

int			main(int ac, char **av, char **termenv)
{
	char	*command;
	t_env	*env;

	(void)ac;
	(void)av;
	env = get_env(termenv);
	// init_termcap
	// ouvrir son fd
	get_current_path(env);
	print_prompt(g_path);
	// signal_set();
	while (get_next_line(0, &command))
	{
		if (ft_strlen(command) > 0)
			go_core(command, &env, &g_path);
		free(command);
		print_prompt(g_path);
	}
	return (0);
}
