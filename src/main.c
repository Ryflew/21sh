/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/18 18:29:37 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/03/25 04:18:01 by vdarmaya         ###   ########.fr       */
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

void	print_prompt(e_state state, char *op)
{
	if (state == BASIC_SHELL)
	{
		ft_putstr("[21sh:");
		ft_putstr(g_sh.prompt);
		ft_putstr("] $> ");
	}
	else
	{
		ft_putchar('\n');
		if (op[ft_strlen(op) - 1] == '\'')
			ft_putstr("quote> ");
		else if (op[ft_strlen(op) - 1] == '"')
			ft_putstr("dquote> ");
		else if (op[ft_strlen(op) - 1] == '`')
			ft_putstr("bquote> ");
		else if (op[ft_strlen(op) - 1] == '(')
			ft_putstr("par> ");
		else if (op[ft_strlen(op) - 1] == '{')
			ft_putstr("aco> ");
		else if (op[ft_strlen(op) - 1] == '[')
			ft_putstr("cro> ");
		else if (op[ft_strlen(op) - 1] == '|')
			ft_putstr("pipe> ");
	}
	get_cursor(&g_sh);
}

static void	sig_hand(int sig)
{
	if (!stop_binary(sig))
	{
		ft_putstr("\n");
		print_prompt(BASIC_SHELL, NULL);
	}
}

static char	shell_loop2(char *command, char **last, e_state *state, char **op)
{
	char	*tmp;

	if (*state == BASIC_SHELL)
	{
		*op = check_quot_brackets(command, state);
		if (*state == ADVANCE_SHELL)
		{
			*last = ft_strjoin(command, "\n");
			return (1);
		}
	}
	if (*state == BRACKET_ERROR)
	{
		ft_putstr("Brackets error !\n");
		*state = BASIC_SHELL;
		if (*last)
			free(*last);
		*last = NULL;
		return (1);
	}
	if (*state == ADVANCE_SHELL)
	{
		treat_second_prompt(command, op, state);
		if (*state == BRACKET_ERROR)
			return (shell_loop2(command, last, state, op));
		if (!**op)
			*state = COMMAND_RUN;
		else
		{
			tmp = *last;
			*last = ft_strdjoin(*last, command, "\n");
			free(tmp);
			return (1);
		}
	}
	return (0);
}
		
static void	shell_loop(t_env **env)
{
	char	*last;
	char	*command;
	char	*tmp;
	char	*op;
	char	buff[3];
	e_state	state;

	op = NULL;
	last = NULL;
	state = BASIC_SHELL;
	while (1)
	{
		command = get_line(&g_sh, buff, &state, op);
		// clear la ligne des espaces !
		if (shell_loop2(command, &last, &state, &op))
		{
			free(command);
			continue ;
		}
		if (!last)
			last = command;
		else
		{
			tmp = last;
			last = ft_strjoin(last, command);
			free(command);
			free(tmp);
		}
		if (*last)
			go_core(last, env, &g_sh);
		ft_putchar('\n');
		free(op);
		free(last);
		op = NULL;
		last = NULL;
		state = BASIC_SHELL;
	}
}

int			main(int ac, char **av, char **termenv)
{
	t_env	*env;

	(void)ac;
	(void)av;
	env = get_env(termenv);
	init_termcap(&g_sh, env);
	get_current_path(env);
	signal(SIGINT, sig_hand);
	// load_history(&g_sh, env);
	shell_loop(&env);
	return (0);
}
