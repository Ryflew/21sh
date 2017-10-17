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

void		delete_lexems(t_list **first_lexems, t_list **lexems)
{
	if (*first_lexems == *lexems)
		*first_lexems = (*lexems)->next;
	ft_pop_node(lexems, (void*)&clear_lexems);
}

void		delete_subshell_lexems(t_list **first_lexems, t_list **lexems,
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

static void	concat_prev_bqt(t_list **lexems, t_token *prev_token, char blank)
{
	t_token *token;

	token = (t_token*)(*lexems)->data;
	if (prev_token && !blank && (prev_token->type == WORD ||
		prev_token->type == NUM))
	{
		free_join(&prev_token->value, VAL);
		ft_pop_node(lexems, (void*)&clear_lexems);
	}
	if (prev_token && prev_token->type == EQUAL)
		TYPE = VAR_WORD;
}

static void	concat_next_bqt(t_list **lexems, t_list *next_lexems)
{
	t_list	*last_new_token;
	t_token	*next_token;

	next_token = (next_lexems) ? (t_token*)next_lexems->data : NULL;
	if (next_token && !next_token->blank && (next_token->type == WORD ||
		next_token->type == NUM))
	{
		last_new_token = *lexems;
		while (last_new_token)
		{
			if (last_new_token->next == next_lexems)
				break ;
			NEXT(last_new_token);
		}
		free_join(&((t_token*)last_new_token->data)->value, next_token->value);
		ft_pop_node(&next_lexems, (void*)&clear_lexems);
	}
}

void		bqt_rule(t_sh *sh, t_list **lexems, t_list **first_lexems,
					char is_cmd)
{
	t_token *prev_token;
	t_list	*next_lexems;
	char	blank;

	prev_token = ((*lexems)->prev) ? (t_token*)(*lexems)->prev->data : NULL;
	blank = ((t_token*)(*lexems)->data)->blank;
	next_lexems = (*lexems)->next;
	while (next_lexems)
	{
		if (((t_token*)next_lexems->prev->data)->type == EBQT)
			break ;
		NEXT(next_lexems);
	}
	delete_lexems(first_lexems, lexems);
	subshell(sh, *lexems, BQT, is_cmd);
	delete_subshell_lexems(first_lexems, lexems, BQT, EBQT);
	if (*lexems)
	{
		if (prev_token)
			concat_prev_bqt(lexems, prev_token, blank);
		if (next_lexems)
			concat_next_bqt(lexems, next_lexems);
	}
}
