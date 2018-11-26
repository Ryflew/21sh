/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/10 20:23:56 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/10/11 11:26:07 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

void		clear_lexems(t_token *token)
{
	free(VAL);
	free(token);
}

void		clear(t_sh *shell, t_list **begin, t_tree *commands_tree)
{
	ft_clear_list(begin, (void*)&clear_lexems);
	if (shell->save_env)
	{
		if (shell->env)
			del_all_env(&(shell->env));
		shell->env = shell->save_env;
		shell->save_env = NULL;
	}
	if (commands_tree)
	{
		if (shell->fds_out)
			ft_clear_list(&shell->fds_out, (&free));
		del_command_tree(commands_tree);
	}
}

void		init_shell_before_parser(t_sh *shell)
{
	shell->fd_pipe = -1;
	shell->fds_out = NULL;
}

void		go_core(char *command, t_sh *shell)
{
	t_tree	*cmds_tree;
	t_list	*begin_lexems;

	get_lexems(shell, shell->total_command, 0);
	add_to_history(shell, command);
	init_shell_before_parser(shell);
	if ((cmds_tree = commands_line_rules(shell, &begin_lexems)) == (void*)-1)
	{
		parse_error(shell);
		clear(shell, &begin_lexems, NULL);
		return ;
	}
	if (cmds_tree)
		browse_tree(cmds_tree, shell, NULL, 1);
	clear(shell, &begin_lexems, cmds_tree);
}
