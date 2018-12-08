/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_subshell_tokens.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/21 14:56:34 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/11/13 14:29:27 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

char			replace_bqt_subshell(t_sh *sh)
{
	t_list	*lexems;
	t_token	*token;

	lexems = sh->lexer->lexems;
	while (lexems)
	{
		token = (t_token*)lexems->data;
		if (TYPE == BQT)
			bqt_rule(sh, &lexems, 1);
		else
			NEXT(lexems);
	}
	return (0);
}

static char		*get_new_tokens(t_sh *sh)
{
	char	*tokens_line;
	char	*to_free;
	int		ret;
	char	buff[501];
	int		*fd;

	fd = (int*)ft_go_end_list(sh->pipe_ss)->data;
	close(fd[1]);
	tokens_line = NULL;
	while ((ret = read(fd[0], buff, 500)) > 0)
	{
		buff[ret] = 0;
		to_free = tokens_line;
		tokens_line = ft_strjoin(tokens_line, buff);
		if (to_free)
			free(to_free);
	}
	if (ret == -1)
	{
		errexit("42sh", "read failure");
		return ((void*)-1);
	}
	return (tokens_line);
}

static void		split_line_to_tokens(char *tokens_line, t_list **new_lexems)
{
	char	**new_tokens;
	int		i;

	if (!(new_tokens = ft_strsplitmulticharacters(tokens_line, "\n ")))
		return ;
	i = -1;
	while (new_tokens[++i])
	{
		ft_node_push_back(new_lexems, new_token(NULL, WORD, new_tokens[i], 1));
		free(new_tokens[i]);
	}
	free(new_tokens);
}

t_list			*add_subshell_tokens(t_sh *sh, enum e_token type, char is_cmd)
{
	t_list	*new_lexems;
	char	*tokens_line;
	char	*trim_line;

	if ((tokens_line = get_new_tokens(sh)) == (void*)-1)
		return (void*)-1;
	new_lexems = NULL;
	if (type == EBQT)
		split_line_to_tokens(tokens_line, &new_lexems);
	else
	{
		trim_line = ft_strtrim(tokens_line);
		if (type == EBQT_INSIDE_ST_OP)
			ft_node_push_back(&new_lexems, new_token(NULL, WORD, trim_line, 1));
		else if (is_cmd && trim_line)
		{
			ft_node_push_back(&new_lexems, new_token(NULL, WORD, "echo", 1));
			ft_node_push_back(&new_lexems, new_token(NULL, WORD, trim_line, 1));
		}
		free(trim_line);
	}
	free(tokens_line);
	return (new_lexems);
}
