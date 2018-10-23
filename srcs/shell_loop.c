/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/02 01:58:38 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/10/23 18:22:06 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "tosh.h"

static void	treat_command(t_sh *shell, char *command)
{
	char	*tmp;

	shell->have_write_error = 0;
	if (!shell->total_command)
		shell->total_command = command;
	else
	{
		tmp = shell->total_command;
		shell->total_command = ft_strjoin(shell->total_command, command);
		free(command);
		free(tmp);
	}
	if (shell->history_mem)
	{
		free(shell->history_mem);
		shell->history_mem = NULL;
	}
	ft_putchar('\n');
	if (*shell->total_command && !only_space(shell->total_command))
		go_core(shell->total_command, shell);
	free(shell->op);
	free(shell->total_command);
	shell->op = NULL;
	shell->total_command = NULL;
	shell->state = BASIC_SHELL;
}

static char	shell_loop3(char **command, char **last, enum e_state *state, \
			char **op)
{
	char	*tmp;

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
			if (*op[ft_strlen(*op) - 1] == '\\')
				*last = ft_strjoin(*last, *command);
			else
				*last = ft_strstrjoin(*last, *command, "\n");
			free(tmp);
			return (1);
		}
	}
	return (0);
}

char		shell_loop2(char **command, char **last, enum e_state *state, \
				char **op)
{
	if (*state == BASIC_SHELL)
	{
		*op = check_quot_brackets(*command, state);
		if (*state == ADVANCE_SHELL)
		{
			if (*op[ft_strlen(*op) - 1] == '\\')
				*last = ft_strdup(*command);
			else
				*last = ft_strjoin(*command, "\n");
			return (1);
		}
	}
	if (*state == BRACKET_ERROR)
	{
		errexit("42sh", "\nBrackets error !");
		*state = BASIC_SHELL;
		if (*last)
			free(*last);
		*last = NULL;
		return (1);
	}
	return (shell_loop3(command, last, state, op));
}

void		shell_loop(t_sh *shell)
{
	char	*command;

	while (1)
	{
		command = get_line(shell, 0, &(shell->state), shell->op);
		if (shell_loop2(&command, &(shell->total_command), \
			&(shell->state), &(shell->op)))
		{
			free(command);
			continue ;
		}
		if (check_history_excla(shell, &command, NULL, NULL))
		{
			if (shell->op)
				ft_strdel(&shell->op);
			if (shell->total_command)
				ft_strdel(&shell->total_command);
			shell->state = BASIC_SHELL;
			ft_putchar('\n');
			continue ;
		}
		if (!shell->have_write_error)
			shell->return_value = 0;
		treat_command(shell, command);
	}
}
