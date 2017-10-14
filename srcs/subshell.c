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

static char	*get_new_tokens(t_sh *sh)
{
	char	*tokens_line;
	char	*to_free;
	int		ret;
	char	buff[501];

	tokens_line = NULL;
	while ((ret = read(sh->pipe_ss[0], buff, 500)) > 0)
	{
		buff[ret] = 0;
		to_free = tokens_line;
		tokens_line = ft_strjoin(tokens_line, buff);
		if (to_free)
			free(to_free);
	}
	if (ret == -1)
	{
		errexit("21sh", "read failure");
		return ((void*)-1);
	}
	return (tokens_line);
}

static char	add_new_tokens(t_sh *sh, enum e_token type)
{
	int		i;
	t_list	*new_lexems;
	char	*tokens_line;
	char	**new_tokens;
	char	*trim_line;

	sh->ssbqt = 0;
	close(sh->pipe_ss[1]);
	if ((tokens_line = get_new_tokens(sh)) == (void*)-1)
		return (0);
	close(sh->pipe_ss[0]);
	new_lexems = NULL;
	if (type == BQT)
	{
		if (!(new_tokens = ft_strsplit(tokens_line, '\n')))
			return (1);
		i = -1;
		while (new_tokens[++i])
		{
			ft_node_push_back(&new_lexems, new_token(NULL, WORD, new_tokens[i]));
			free(new_tokens[i]);
		}
		free(new_tokens);
	}
	else
	{
		trim_line = ft_strtrim(tokens_line);
		ft_node_push_back(&new_lexems, new_token(NULL, WORD, "echo"));
		ft_node_push_back(&new_lexems, new_token(NULL, WORD, trim_line));
		free(trim_line);
	}
	free(tokens_line);
	if (new_lexems)
		ft_add_list(sh->lexer->lexems, new_lexems);
	return (1);
}

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
		manage_subshell(sh, sub_tree);
		exit(ret);
	}
	else
		waitpid(father, &ret, 0);

}

char		subshell(t_sh *sh, t_list *lexems, enum e_token type)
{
	t_tree	*sub_tree;

	sh->lexer->lexems = lexems;
	sh->current_token = (t_token*)lexems->data;
	if ((sub_tree = commands_line_rules(sh)) == (void*)-1 || !sub_tree)
	{
		return (0);
		parse_error(sh);
	}
	sh->ssbqt = 1;
	sh->pipe_ss[0] = 0;
	get_fd(sh, sh->pipe_ss);
	fork_subshell(sh, sub_tree);
	del_command_tree(sub_tree);
	return (add_new_tokens(sh, type));
}
