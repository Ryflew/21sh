/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_variable.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdurst <bdurst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/11 17:45:15 by bdurst            #+#    #+#             */
/*   Updated: 2017/10/11 17:45:16 by bdurst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

static void	free_lexems(t_list **lexems, t_list **cmd_tokens)
{
	if (*lexems == *cmd_tokens)
		*cmd_tokens = (*lexems)->next;
	ft_pop_node(lexems, (void*)&clear_lexems);
}

void		concat_unused_add_var(t_list *cmd_tokens)
{
	t_list	*tmp;
	t_token	*token;
	t_token	*next_token;

	tmp = cmd_tokens;
	while (tmp)
	{
		token = (t_token*)tmp->data;
		if (tmp->next)
			next_token = (t_token*)tmp->next->data;
		if (tmp->next && TYPE == VAR_WORD && (next_token->type == EQUAL \
			|| next_token->type == VAR_WORD))
		{
			free_join(&VAL, next_token->value);
			free_lexems(&tmp->next, &cmd_tokens);
		}
		else
		{
			if (TYPE == VAR_WORD)
				TYPE = WORD;
			NEXT(tmp);
		}
	}
}

char		is_env_var_to_add(t_list **cmd_tokens, t_list *tmp, char ret)
{
	t_token	*token;
	char	*var_content;

	while (tmp && !ret)
	{
		token = (t_token*)tmp->data;
		if (TYPE != EQUAL && TYPE != VAR_WORD)
			ret = 1;
		else if (TYPE == EQUAL && tmp->prev && \
			((t_token*)tmp->prev->data)->type == VAR_WORD)
		{
			var_content = "";
			if (tmp->next && ((t_token*)tmp->next->data)->type == VAR_WORD)
				var_content = ((t_token*)tmp->next->data)->value;
			add_var_twoline(((t_token*)tmp->prev->data)->value, \
				var_content, &(get_shell()->shell_var));
			free_lexems(&tmp->prev, cmd_tokens);
			free_lexems(&tmp, cmd_tokens);
			if (tmp && ((t_token*)tmp->data)->type == VAR_WORD)
				free_lexems(&tmp, cmd_tokens);
		}
		else
			NEXT(tmp);
	}
	return (ret);
}
