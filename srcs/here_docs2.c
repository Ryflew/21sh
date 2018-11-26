/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_docs2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/11 15:20:09 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/09/18 17:16:49 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

static void	manage_here_doc_bqt0(t_sh *sh)
{
	eat(sh, BQT);
	if (!(subshell(sh, sh->lexer->lexems, EBQT, 1)))
		return ;
	if (eat(sh, EBQT) == -1)
	{
		errexit("42sh", "parse error: backquote isn't close");
		return ;
	}
}

char		manage_here_doc_bqt(t_sh *sh, t_list *end_bqt, t_list **tmp, \
				t_list **begin_lexems)
{
	t_token	*token;
	char	join;

	manage_here_doc_bqt0(sh);
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
