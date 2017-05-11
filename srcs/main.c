/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/18 18:29:37 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/05/11 03:38:21 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "21sh.h"

static t_sh	g_sh;

static void	get_current_path(t_env *env)
{
	char	*cwd;
	char	buff[4097];

	cwd = getcwd(buff, 4097);
	g_sh.prompt = get_with_tilde(cwd, env);
}

t_sh	*get_shell()
{
	return (&g_sh);
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
		move_to(g_sh.pos.last.x, g_sh.pos.last.y);
		ft_putstr("\n");
		print_prompt(BASIC_SHELL, NULL);
	}
}

char	*remove_useless_space(char *str)
{
	int		i;
	int		j;
	int		have_space;
	char	buff[ft_strlen(str)];
	char	c;

	have_space = 0;
	j = -1;
	i = -1;
	c = 0;
	while (str[++i])
	{
		if (c)
			buff[++j] = str[i];
		else if (!ft_isblank(str[i]))
		{
			if (have_space && j != -1)
				buff[++j] = ' ';
			have_space = 0;
			buff[++j] = str[i];
		}
		else if (!c && ft_isblank(str[i]))
			have_space = 1;
		if (!c && (str[i] == '\'' || str[i] == '"' || str[i] == '`'))
			c = str[i];
		else if (c == str[i])
			c = '\0';
	}
	buff[++j] = '\0';
	return (ft_strdup(buff));
}

static void	init_shell(t_sh *shell, t_lexer *lexer)
{
	shell->ref_pos = -1;
	shell->saved = NULL;
	shell->history_pos = -1;
	shell->history_mem = NULL;
	lexer->string_operator = 0;
	lexer->red = 0;
	shell->lexer = lexer;
	shell->pipe = NULL;
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
