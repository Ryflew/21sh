/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdurst <bdurst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/11 17:45:03 by bdurst            #+#    #+#             */
/*   Updated: 2017/10/11 17:45:04 by bdurst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

static void	manage_subshell(t_sh *sh, t_tree *sub_tree)
{
	sh->fd_pipe = -1;
	sh->fds_out = NULL;
	sh->return_value = 0;
	browse_tree(sub_tree, sh, NULL, 1);
	if (sub_tree)
	{
		if (sh->fds_out)
			ft_clear_list(&sh->fds_out, (&free));
		sh->fds_out = NULL;
		sh->fd_pipe = -1;
		del_command_tree(sub_tree);
	}
}

static void	fork_subshell(t_sh *sh, t_tree *sub_tree)
{
	int		ret;
	pid_t	father;

	if ((father = fork()) == -1)
		ft_exiterror("fork failure !", -1);
	else if (!father)
	{
		ret = EXIT_SUCCESS;
		sh->ssbqt = 1;
		manage_subshell(sh, sub_tree);
		sh->ssbqt = 0;
		exit(ret);
	}
	else
		waitpid(father, &ret, 0);
}

char		subshell(t_sh *sh, t_list *lexems, enum e_token type)
{
	t_tree	*sub_tree;
	int		pipe_ss[2];
	t_list	*last_pipe;

	sh->lexer->lexems = lexems;
	sh->current_token = (t_token*)lexems->data;
	if ((sub_tree = commands_line_rules(sh)) == (void*)-1 || !sub_tree)
	{
		return (0);
		parse_error(sh);
	}
	if ((pipe(pipe_ss)) == -1)
	{
		errexit("21sh", "pipe failure !\n");
		return (0);
	}
	ft_node_push_back(&sh->pipe_ss, pipe_ss);
	fork_subshell(sh, sub_tree);
	del_command_tree(sub_tree);
	add_subshell_tokens(sh, type);
	last_pipe = ft_go_end_list(sh->pipe_ss);
	if (last_pipe == sh->pipe_ss)
		sh->pipe_ss = NULL;
	ft_pop_node(&last_pipe, NULL);
	return (1);
}
