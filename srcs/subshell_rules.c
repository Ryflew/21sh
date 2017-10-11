/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell_rules.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdurst <bdurst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/11 17:45:09 by bdurst            #+#    #+#             */
/*   Updated: 2017/10/11 17:45:10 by bdurst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

t_token			*par_rule(t_sh *sh)
{
	t_token *token;

	eat(sh, LPAR);
	subshell(sh, LPAR);
	if (!sh->current_token || eat(sh, RPAR) == -1)
	{
		errexit("21sh", "parse error: miss close par `)`");
		return ((void*)-1);
	}
	if (!(token = text_rules(sh)))
		return (new_token(NULL, NONE, ""));
	return (token);
}

static t_token	*concat_bqt(t_sh *sh, char concat,
					t_token *prev_token)
{
	t_token *token;

	if (!(subshell(sh, BQT)))
		return ((void*)-1);
	if (!sh->current_token || eat(sh, EBQT) == -1)
	{
		errexit("21sh", "parse error: miss close backquote '`'");
		return ((void*)-1);
	}
	if (!(token = text_rules(sh)))
		return (new_token(NULL, NONE, ""));
	else if (concat && prev_token && (prev_token->type == WORD ||
	prev_token->type == NUM || prev_token->type == TILD_VAR_WORD ||
	prev_token->type == TILD || prev_token->type == VAR_WORD))
	{
		free_join(&prev_token->value, VAL);
		token = prev_token;
	}
	else if (prev_token && prev_token->type == EQUAL)
		token->type = VAR_WORD;
	return (token);
}

t_token			*bqt_rule(t_sh *sh)
{
	char	concat;
	t_token	*prev_token;

	prev_token = NULL;
	concat = 0;
	if (sh->lexer->lexems->prev)
		prev_token = (t_token*)sh->lexer->lexems->prev->data;
	if (sh->current_token->type == BQT)
		eat(sh, BQT);
	else
	{
		eat(sh, BQT_C);
		concat = 1;
	}
	return (concat_bqt(sh, concat, prev_token));
}
