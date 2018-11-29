/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/11 17:45:03 by bdurst            #+#    #+#             */
/*   Updated: 2018/11/29 15:05:34 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

static t_tree	*lex_and_parse_subshell(t_sh *sh, t_list *lexems, \
				t_list **begin_lexems)
{
	t_tree	*sub_tree;

	get_lexems(sh, ((t_token*)lexems->data)->value, 0);
	sh->lexer->line = sh->total_command;
	sh->lexer->her = 0;
	if (!(sub_tree = commands_line_rules(sh, begin_lexems)) \
	|| sub_tree == (void*)-1)
	{
		clear_lexer_parser(sh, begin_lexems, NULL);
		if (sub_tree == (void*)-1)
			parse_error(sh);
		return (NULL);
	}
	return (sub_tree);
}

static void		manage_subshell(t_sh *sh, t_list *lexems)
{
	t_list	*begin_lexems;
	t_tree	*sub_tree;

	sh->fd_pipe = -1;
	sh->fds_out = NULL;
	sh->return_value = 0;
	if (!(sub_tree = lex_and_parse_subshell(sh, lexems, &begin_lexems)))
		return ;
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

static void		fork_subshell(t_sh *sh, t_list *lexems)
{
	int		ret;
	pid_t	father;

	if ((father = fork()) == -1)
		ft_exiterror("fork failure !", -1);
	else if (!father)
	{
		ret = EXIT_SUCCESS;
		sh->ssbqt = BQT;
		manage_subshell(sh, lexems);
		sh->ssbqt = 0;
		exit(ret);
	}
	else
		waitpid(father, &ret, 0);
}

static t_list	*subshell_pipe(t_sh *sh, enum e_token type, char is_cmd, \
					t_list *lexems)
{
	int		pipe_ss[2];
	t_list	*last_pipe;
	t_list	*new_lexems;

	if ((pipe(pipe_ss)) == -1)
	{
		errexit("42sh", "pipe failure !\n");
		return ((void*)-1);
	}
	ft_node_push_back(&sh->pipe_ss, pipe_ss);
	fork_subshell(sh, lexems);
	new_lexems = add_subshell_tokens(sh, type, is_cmd);
	last_pipe = ft_go_end_list(sh->pipe_ss);
	if (last_pipe == sh->pipe_ss)
		sh->pipe_ss = NULL;
	ft_pop_node(&last_pipe, NULL);
	return (new_lexems);
}

char			subshell(t_sh *sh, t_list *lexems, enum e_token type, \
					char is_cmd)
{
	t_list	*new_lexems;
	t_list	*end_bqt_lexem;

	if ((new_lexems = subshell_pipe(sh, type, is_cmd, lexems)) == (void*)-1)
		return (0);
	if (new_lexems && sh->lexer->lexems->next)
	{
		end_bqt_lexem = sh->lexer->lexems->next;
		while (end_bqt_lexem && ((t_token*)end_bqt_lexem->data)->type != type)
			end_bqt_lexem = end_bqt_lexem->next;
		ft_add_list(end_bqt_lexem, new_lexems);
	}
	return (1);
}
