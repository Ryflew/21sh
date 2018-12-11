/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_docs2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdurst2812 <bdurst2812@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/11 15:20:09 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/12/11 23:28:49 by bdurst2812       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

static void	manage_here_doc_bqt0(t_sh *sh, enum e_token end_type, \
								t_list **begin_lexems)
{
	delete_first_subshell_lexems(begin_lexems, &sh->lexer->lexems);
	if (!(subshell(sh, sh->lexer->lexems, end_type, 1)))
		return ;
	if (!delete_subshell_lexems(begin_lexems, &sh->lexer->lexems, BQT, end_type))
		errexit("42sh", "parse error: backquote isn't close");
}

char		manage_here_doc_bqt(t_sh *sh, t_list *end_bqt, \
				t_list **begin_lexems)
{
	t_token	*end_token;

	if (!end_bqt)
		return (0);
	end_token = (t_token*)end_bqt->data;
	manage_here_doc_bqt0(sh, end_token->type, begin_lexems);
	return (1);
}
