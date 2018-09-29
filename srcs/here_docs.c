/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_docs.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/10 20:26:31 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/09/29 14:17:31 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

static char	is_bqt_in_heredoc(t_sh *sh, t_list **begin_lexems)
{
	t_list	*tmp;
	t_token	*token;
	t_list	*end_bqt;

	tmp = *begin_lexems;
	while (tmp)
	{
		token = (t_token*)tmp->data;
		if (TYPE == BQT)
		{
			sh->current_token = token;
			sh->lexer->lexems = tmp;
			while (tmp && ((t_token*)tmp->data)->type != EBQT)
				NEXT(tmp);
			if (tmp)
				end_bqt = tmp->next;
			tmp = sh->lexer->lexems;
			manage_here_doc_bqt(sh, end_bqt, &tmp, begin_lexems);
		}
		if (tmp)
			NEXT(tmp);
	}
	return (1);
}

static void	lex_parse_heredoc2(t_list **begin_lexems, t_sh *sh, char **output, \
			char *tmp)
{
	char	**cmds;
	int		j;

	cmds = get_cmds(begin_lexems, sh);
	j = -1;
	while (cmds[++j])
	{
		if (j > 0)
			free_join(output, " ");
		free_join(output, cmds[j]);
	}
	if (tmp)
		free_join(output, "\n");
	free_join(output, cmds[j]);
	ft_strdelpp(&cmds);
	ft_clear_list(begin_lexems, (void*)&clear_lexems);
}

static char	*lex_parse_heredoc(t_sh *sh, char *heredoc_line, int i)
{
	t_list	*begin_lexems;
	char	**line;
	char	*output;

	line = ft_strsplit(heredoc_line, '\n');
	sh->lexer->her = 1;
	output = NULL;
	while (line[++i])
	{
		sh->lexer->line = line[i];
		get_lexems(sh);
		begin_lexems = sh->lexer->lexems;
		if (!(is_bqt_in_heredoc(sh, &begin_lexems)))
		{
			ft_clear_list(&begin_lexems, (void*)&clear_lexems);
			sh->lexer->her = 0;
			return ((void*)-1);
		}
		lex_parse_heredoc2(&begin_lexems, sh, &output, line[i + 1]);
	}
	free(heredoc_line);
	ft_strdelpp(&line);
	sh->lexer->her = 0;
	return (output);
}

char		*read_here_doc(char *cmd, char *prompt)
{
	char	*line;
	char	*to_free;
	int		ret;
	char	*heredoc_line;

	line = NULL;
	heredoc_line = NULL;
	ft_putstr(prompt);
	while ((ret = get_next_line(0, &line) > 0) && ft_strcmp(line, cmd))
	{
		to_free = heredoc_line;
		heredoc_line = (!to_free) ? ft_strdup(line) : \
			ft_strstrjoin(to_free, "\n", line);
		if (to_free)
			ft_strdel(&to_free);
		ft_strdel(&line);
		ft_putstr(prompt);
	}
	free(line);
	if (ret == -1)
		ft_exiterror("read failure", -1);
	return (heredoc_line);
}

char		manage_here_doc(t_sh *sh, char *heredoc_line, t_tree *node,
				int *fd_pipe)
{
	char	*output;

	if ((output = (lex_parse_heredoc(sh, heredoc_line, -1))) == (void*)-1)
		return (0);
	if (output && node->left)
	{
		ft_fputendl(output, fd_pipe[1]);
		close(fd_pipe[1]);
	}
	free(output);
	return (1);
}
