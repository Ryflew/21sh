/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/10 20:27:54 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/10/10 20:27:56 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

static void		init_lexer(t_lexer *lexer)
{
	lexer->lexems = NULL;
	lexer->brc = 0;
	lexer->bkt = 0;
	lexer->bqt = 0;
	lexer->blank = 0;
	lexer->red = 0;
}

char			isnt_here_or_bqt(t_lexer *lexer)
{
	return (!lexer->her || lexer->bqt);
}

static t_token	*is_end_expr(t_lexer *lexer, t_token *l_tk)
{
	if (isnt_here_or_bqt(lexer) && l_tk && (is_glob_token(l_tk->type)
	|| l_tk->type == EXPR || l_tk->type == TILD_EXPR || l_tk->type == BKT_EXPR
	|| l_tk->type == COM))
		return (new_token(lexer, END_EXPR, ""));
	else if (l_tk && l_tk->type == ASCII_WORD)
		l_tk->type = WORD;
	return (NULL);
}

static t_token	*get_next_token(t_lexer *lexer, t_token *last_token)
{
	t_token *token;

	token = NULL;
	while (*lexer->line)
	{
		token = NULL;
		if (ft_isblank(*lexer->line))
		{
			while (*lexer->line && ft_isblank(*lexer->line))
				++lexer->line;
			lexer->blank = 1;
			if ((token = is_end_expr(lexer, last_token)))
				return (token);
		}
		else if ((token = identify_token(lexer, last_token)))
		{
			lexer->blank = 0;
			if (last_token && last_token->type == ASCII_WORD)
				last_token->type = WORD;
			return (token);
		}
	}
	return (is_end_expr(lexer, last_token));
}

void			get_lexems(t_sh *sh)
{
	t_token *token;

	init_lexer(sh->lexer);
	token = NULL;
	while ((token = get_next_token(sh->lexer, token)))
		ft_node_push_back(&(sh->lexer->lexems), token);
}
