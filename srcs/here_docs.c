/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_docs.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/10 20:26:31 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/10/10 20:31:33 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

static char	manage_here_doc_bqt(t_sh *sh, t_list *end_bqt, t_list **tmp, t_list **begin_lexems)
{
	t_token	*token;
	char	join;

	if (eat(sh, BQT) == -1)
		eat(sh, BQT_C);
	if (!(subshell(sh, BQT)))
		return (0);
	if (eat(sh, EBQT) == -1)
	{
		ft_putendl("parse error: backquote isn't close");
		return (0);
	}
	join = 0;
	while (*tmp && (*tmp)->next && (*tmp)->next != end_bqt)
	{
		token = (t_token*)(*tmp)->data;
		if (*begin_lexems == *tmp && !join)
			*begin_lexems = (*tmp)->next;
		if (TYPE == EBQT)
			join = 1;
		else if (join)
		{
			if ((*tmp)->next->next != end_bqt)
				free_join(&VAL, "\n");
			free_join(&VAL, ((t_token*)(*tmp)->next->data)->value);
			ft_pop_node(&(*tmp)->next, (void*)&clear_lexems);
		}
		if (!join || TYPE == EBQT)
			ft_pop_node(tmp, (void*)&clear_lexems);
	}
	return (1);
}

static char	is_bqt_in_heredoc(t_sh *sh, t_list **begin_lexems)
{
	t_list	*tmp;
	t_token	*token;
	t_list	*end_bqt;

	tmp = *begin_lexems;
	while (tmp)
	{
		token = (t_token*)tmp->data;
		if (TYPE == BQT || TYPE == BQT_C)
		{
			sh->current_token = token;
			sh->lexer->lexems = tmp;
			while (tmp && ((t_token*)tmp->data)->type != EBQT)
				NEXT(tmp);
			if (tmp)
				end_bqt = tmp->next;
				tmp = sh->lexer->lexems;
			if (!(manage_here_doc_bqt(sh, end_bqt, &tmp, begin_lexems)))
				return (0);
		}
		if (tmp)
			NEXT(tmp);
	}
	return (1);
}

static char	*lex_parse_heredoc(t_sh *sh, char *heredoc_line)
{
	t_list	*begin_lexems;
	char	**cmds;
	char	**line;
	int		i;
	int		j;
	char	*output;
	
	line = ft_strsplit(heredoc_line, '\n');
	i = -1;
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
		cmds = get_cmds(&begin_lexems, sh);
		j = -1;
		while (cmds[++j])
		{
			if (j > 0)
				free_join(&output, " ");
			free_join(&output, cmds[j]);
		}
		if (line[i + 1])
			free_join(&output, "\n");		
		free_join(&output, cmds[j]);		
		ft_strdelpp(&cmds);
		/*t_list *tmp = begin_lexems;
		t_token *token;
		while (tmp)
		{
			token = (t_token*)tmp->data;
			ft_putendl(VAL);
			NEXT(tmp);
		}*/
		ft_clear_list(&begin_lexems, (void*)&clear_lexems);
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
	while ((ret = gnl(0, &line) > 0) && ft_strcmp(line, cmd))
	{
		to_free = heredoc_line;
		heredoc_line = (!to_free) ? ft_strdup(line) : ft_strstrjoin(to_free, "\n", line);
		if (to_free)
			free(to_free);
		free(line);
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

	if ((output = (lex_parse_heredoc(sh, heredoc_line))) == (void*)-1)
		return (0);
	if (output && node->left)
		ft_fputendl(output, fd_pipe[1]);
	free(output);
	return (1);
}
