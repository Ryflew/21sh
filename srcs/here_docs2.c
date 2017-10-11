/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_docs2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/11 15:20:09 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/10/11 15:21:01 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

static char	manage_here_doc_bqt0(t_sh *sh)
{
	if (eat(sh, BQT) == -1)
		eat(sh, BQT_C);
	if (!(subshell(sh, BQT)))
		return (0);
	if (eat(sh, EBQT) == -1)
	{
		ft_putendl("parse error: backquote isn't close");
		return (0);
	}
	return (1);
}

char		manage_here_doc_bqt(t_sh *sh, t_list *end_bqt, t_list **tmp, \
				t_list **begin_lexems)
{
	t_token	*token;
	char	join;

	if (!manage_here_doc_bqt0(sh))
		return (0);
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
