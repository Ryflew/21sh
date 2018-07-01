/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/11 17:46:04 by bdurst            #+#    #+#             */
/*   Updated: 2018/07/01 22:28:02 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

static char		**create_tmp_env(t_sh *sh, int size, t_token *token)
{
	char	**tmp_env;
	int		i;

	if (!token)
		sh->current_token = NULL;
	else if (size == 1)
		token = text_rules(sh, 0);
	else
	{
		if (ft_strcmp(VAL, "env"))
			++size;
		if (!(tmp_env = (char**)malloc(sizeof(char*) * size)))
			ft_exiterror("Malloc failed", 1);
		tmp_env[size - 1] = NULL;
		i = 0;
		--size;
		while (i < size)
		{
			token = text_rules(sh, 0);
			tmp_env[i++] = VAL;
		}
		return (tmp_env);
	}
	return (NULL);
}

static t_list	*increase(t_list *tmp, int *size, enum e_token type)
{
	if (type != VAR_WORD && type != EQUAL)
		++(*size);
	return (tmp->next);
}

static t_token	*compute_tmp_env_size(t_list **tmp, int *size, t_token *token)
{
	t_token	*prev_token;

	if ((*tmp)->prev)
		prev_token = (t_token*)(*tmp)->prev->data;
	if ((*tmp)->prev && TYPE == EQUAL && (prev_token->type == VAR_WORD))
	{
		if ((*tmp)->next && ((t_token*)(*tmp)->next->data)->type == VAR_WORD)
			free_join(&VAL, ((t_token*)(*tmp)->next->data)->value);
		ft_pop_node(&(*tmp)->next, (void*)&clear_lexems);
		free_join(&prev_token->value, VAL);
		prev_token->type = WORD;
		*tmp = (*tmp)->prev;
		ft_pop_node(&(*tmp)->next, (void*)&clear_lexems);
	}
	else
	{
		*tmp = increase(*tmp, size, TYPE);
		if (*tmp && !ft_strcmp(VAL, "-u") \
		&& (((t_token*)(*tmp)->data)->type == WORD \
		|| ((t_token*)(*tmp)->data)->type == NUM))
			*tmp = increase(*tmp, size, NONE);
	}
	if (*tmp)
		token = (*tmp)->data;
	return (token);
}

char			**parse_env_cmds(t_sh *sh)
{
	int		size;
	t_list	*tmp;
	t_token	*token;

	tmp = sh->lexer->lexems;
	if (tmp->next)
		token = tmp->next->data;
	if (!sh->current_token || ft_strcmp(sh->current_token->value, "env")
		|| !tmp->next || (TYPE != WORD && TYPE != NUM && TYPE != TILD && \
		TYPE != VAR_WORD))
		return (NULL);
	tmp = tmp->next;
	size = 1;
	while (tmp && (!ft_strcmp(VAL, "-i") || !ft_strcmp(VAL, "-u")
		|| !ft_strcmp(VAL, "env") ||\
		ft_strchr(VAL, '=') || TYPE == VAR_WORD || TYPE == EQUAL))
		token = compute_tmp_env_size(&tmp, &size, token);
	return (create_tmp_env(sh, size, token));
}
