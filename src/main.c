/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/18 18:29:37 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/03/27 21:12:54 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
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

void	sig_hand(int sig)
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
		ft_putstr("\n");
		print_prompt(BASIC_SHELL, NULL);
		g_sh.pos.first = (t_pos){g_sh.pos.cursor.x, g_sh.pos.cursor.y};
		g_sh.pos.last = (t_pos){g_sh.pos.cursor.x, g_sh.pos.cursor.y};
	}
}

char	*remove_useless_space(char *str, char c)
{
	int		i;
	int		j;
	int		have_space;
	char	buff[ft_strlen(str)];

	have_space = 0;
	j = -1;
	i = -1;
	while (str[++i])
	{
		if (c)
			buff[++j] = str[i];
		else if (str[i] != ' ' && str[i] != '\t')
		{
			if (have_space && j != -1)
				buff[++j] = ' ';
			have_space = 0;
			buff[++j] = str[i];
		}
		else if (!c && (str[i] == ' ' || str[i] == '\t'))
			have_space = 1;
		if (!c && (str[i] == '\'' || str[i] == '"' || str[i] == '`'))
			c = str[i];
		else if (c == str[i])
			c = '\0';
	}
	buff[++j] = '\0';
	return (ft_strdup(buff));
}

static char	shell_loop2(char **command, char **last, e_state *state, char **op)
{
	char	*tmp;

	if (*state == BASIC_SHELL)
	{
		*op = check_quot_brackets(*command, state);
		if (*state == ADVANCE_SHELL)
		{
			*last = ft_strjoin(*command, "\n");
			return (1);
		}
	}
	if (*state == BRACKET_ERROR)
	{
		ft_putstr("\nBrackets error !\n");
		*state = BASIC_SHELL;
		if (*last)
			free(*last);
		*last = NULL;
		return (1);
	}
	if (*state == ADVANCE_SHELL)
	{
		treat_second_prompt(*command, op, state);
		if (*state == BRACKET_ERROR)
			return (shell_loop2(command, last, state, op));
		if (!**op)
			*state = COMMAND_RUN;
		else
		{
			tmp = *last;
			*last = ft_strdjoin(*last, *command, "\n");
			free(tmp);
			return (1);
		}
	}
	return (0);
}

static void	shell_loop(t_env **env)
{
	char	*command;
	char	*tmp;
	char	buff[3];

	g_sh.op = NULL;
	g_sh.total_command = NULL;
	g_sh.state = BASIC_SHELL;
	while (1)
	{
		command = get_line(&g_sh, buff, &(g_sh.state), g_sh.op);
		tmp = command;
		command = remove_useless_space(command, !g_sh.op ? 0 : g_sh.op[ft_strlen(g_sh.op) - 1]);
		free(tmp);
		if (shell_loop2(&command, &(g_sh.total_command), &(g_sh.state), &(g_sh.op)))
		{
			free(command);
			continue ;
		}
		if (!g_sh.total_command)
			g_sh.total_command = command;
		else
		{
			tmp = g_sh.total_command;
			g_sh.total_command = ft_strjoin(g_sh.total_command, command);
			free(command);
			free(tmp);
		}
		if (*g_sh.total_command)
			go_core(g_sh.total_command, env, &g_sh);
		ft_putchar('\n');
		free(g_sh.op);
		free(g_sh.total_command);
		g_sh.op = NULL;
		g_sh.total_command = NULL;
		g_sh.state = BASIC_SHELL;
	}
}

int			main(int ac, char **av, char **termenv)
{
	(void)ac;
	(void)av;
	g_sh.env = get_env(termenv);
	init_termcap(&g_sh, g_sh.env);
	get_current_path(g_sh.env);
	signal(SIGINT, sig_hand);
	load_history(&g_sh, g_sh.env);
	shell_loop(&(g_sh.env));
	return (0);
}
