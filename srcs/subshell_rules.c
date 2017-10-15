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

static void	delete_lexems(t_list **first_lexems, t_list **lexems)
{
	if (*first_lexems == *lexems)
		*first_lexems = (*lexems)->next;
	ft_pop_node(lexems, (void*)&clear_lexems);
}

static void	delete_subshell_lexems(t_list **first_lexems, t_list **lexems,
								enum e_token open_type, enum e_token close_type)
{
	t_token	*token;
	char	open;

	open = 0;
	while (*lexems)
	{
		token = (t_token*)(*lexems)->data;
		if (TYPE == open_type)
			++open;
		if (TYPE == close_type)
		{
			if (!open)
			{
				delete_lexems(first_lexems, lexems);
				break ;
			}
			--open;
		}
		delete_lexems(first_lexems, lexems);
	}
}

void		par_rule(t_sh *sh, t_list **lexems, t_list **first_lexems)
{
	delete_lexems(first_lexems, lexems);
	subshell(sh, *lexems, LPAR);
	delete_subshell_lexems(first_lexems, lexems, LPAR, RPAR);
}

static void	concat_bqt(t_sh *sh, t_list **lexems, t_list **first_lexems,
					char concat)
{
	t_token *token;
	t_token *prev_token;

	if ((*lexems)->prev)
		prev_token = (t_token*)(*lexems)->data;
	delete_lexems(first_lexems, lexems);
	subshell(sh, *lexems, BQT);
	delete_subshell_lexems(first_lexems, lexems, BQT, EBQT);
	if (*lexems)
	{
		token = (t_token*)(*lexems)->data;
		if (concat && prev_token && (prev_token->type == WORD ||
		prev_token->type == NUM || prev_token->type == TILD_VAR_WORD ||
		prev_token->type == TILD || prev_token->type == VAR_WORD))
			free_join(&prev_token->value, VAL);
		else if (prev_token && prev_token->type == EQUAL)
			TYPE = VAR_WORD;
	}
}

void		bqt_rule(t_sh *sh, t_list **lexems, t_list **first_lexems)
{
	char	concat;
	t_token	*prev_token;
	t_token	*token;

	prev_token = NULL;
	concat = 0;
	token = (t_token*)(*lexems)->data;
	if (TYPE == BQT_C)
		concat = 1;
	concat_bqt(sh, lexems, first_lexems, concat);
}
