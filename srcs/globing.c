/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/10 20:26:01 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/11/29 14:53:27 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

static void	replace_expr_by_word(t_list **lexems, t_list *tmp)
{
	t_token *token;

	while (tmp)
	{
		token = (t_token*)tmp->data;
		if (TYPE == END_EXPR)
		{
			if (*lexems == tmp)
				*lexems = tmp->next;
			ft_pop_node(&tmp, (void*)&clear_lexems);
		}
		else
		{
			if (TYPE == EXPR)
				TYPE = WORD;
			if (TYPE == SON)
			{
				if (*lexems == tmp)
					*lexems = tmp->next;
				ft_pop_node(&tmp, (void*)&clear_lexems);
			}
			else
				tmp = tmp->next;
		}
	}
}

static void	init_checker(char *brc, char *bkt, char *pb)
{
	*brc = 0;
	*bkt = 0;
	*pb = 0;
}

static char	check_bad_or_not(t_list *lxms, t_token *token, char *brc,
				char *bkt)
{
	if (TYPE == LBRC)
	{
		if (*bkt || (((t_token*)(lxms->next->data))->type == RBRC && *brc < 1))
			return (1);
		++(*brc);
	}
	else if (TYPE == COM && (*bkt || !*brc))
		return (1);
	else if (TYPE == RBRC)
	{
		if (*bkt || !*brc)
			return (1);
		--(*brc);
	}
	else if (TYPE == LBKT || TYPE == E_WILDCARD)
	{
		if (*bkt || ((t_token*)(lxms->next->data))->type == RBKT)
			return (1);
		*bkt = 1;
	}
	else if (TYPE == RBKT && !*bkt)
		return (1);
	else if (TYPE == RBKT)
		*bkt = 0;
	return (0);
}

static void	clear_bad_expr(t_list *lexems)
{
	t_token	*token;
	t_list	*save;
	char	brc;
	char	bkt;
	char	pb;

	save = NULL;
	init_checker(&brc, &bkt, &pb);
	while (lexems)
	{
		token = (t_token*)lexems->data;
		if (!save && (is_glob_token(TYPE) || TYPE == EXPR))
			save = lexems;
		else if (TYPE == END_EXPR)
		{
			if (brc || bkt || pb)
				merge_expr_to_word(save);
			init_checker(&brc, &bkt, &pb);
			save = NULL;
		}
		if (!pb)
			pb = check_bad_or_not(lexems, token, &brc, &bkt);
		lexems = lexems->next;
	}
}

void		glob(t_list **first_lexems)
{
	t_list	*lexems;
	t_token	*token;

	lexems = *first_lexems;
	clear_bad_expr(lexems);
	while (lexems)
	{
		token = (t_token*)lexems->data;
		if (TYPE == LBRC)
		{
			manage_brc(lexems->next);
			clear_old_expr(&lexems, first_lexems, 1);
			merge_expr(*first_lexems);
		}
		else
			lexems = lexems->next;
	}
	replace_all_exprs(first_lexems);
	replace_expr_by_word(first_lexems, *first_lexems);
}
