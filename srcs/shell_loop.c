/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/02 01:58:38 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/04/02 03:41:22 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "21sh.h"

static void	treat_command(t_sh *shell, char *command)
{
	char	*tmp;

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
	if (*shell->total_command)
		go_core(shell->total_command, &(shell->env), shell);
	ft_putchar('\n');
	free(shell->op);
	free(shell->total_command);
	shell->op = NULL;
	shell->total_command = NULL;
	shell->state = BASIC_SHELL;
}

static char	shell_loop3(char **command, char **last, e_state *state, char **op)
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
			*last = ft_strstrjoin(*last, *command, "\n");
			free(tmp);
			return (1);
		}
	}
	return (0);
}

char		shell_loop2(char **command, char **last, e_state *state, char **op)
{
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
	return (shell_loop3(command, last, state, op));
}

void		shell_loop(t_sh *shell)
{
	char	*command;
	//char	*tmp;

	shell->op = NULL;
	shell->total_command = NULL;
	shell->state = BASIC_SHELL;
	while (1)
	{
		command = get_line(shell, 0, &(shell->state), shell->op);
		//tmp = command;
		//command = remove_useless_space(command, !shell->op ? 0 : \
//			shell->op[ft_strlen(shell->op) - 1]);
		//free(tmp);
		if (shell_loop2(&command, &(shell->total_command), \
			&(shell->state), &(shell->op)))
		{
			free(command);
			continue ;
		}
		treat_command(shell, command);
	}
}