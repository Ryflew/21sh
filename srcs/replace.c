/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/11 17:45:24 by bdurst            #+#    #+#             */
/*   Updated: 2018/11/15 14:54:29 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

static void	concat_var(t_list **lexems, t_token *token, t_token *last_token,
						char check_last)
{
	char	*to_free;
	t_list	*last_lexems;

	if (check_last && last_token->type != WORD && last_token->type != NUM && last_token->type != NEG_NUM)
		return ;
	else if (!check_last && TYPE != WORD && TYPE != NUM && TYPE != NEG_NUM)
		return ;
	to_free = last_token->value;
	last_token->value = ft_strjoin(to_free, VAL);
	free(to_free);
	last_lexems = (*lexems)->prev;
	ft_pop_node(lexems, (void*)&clear_lexems);
	if ((*lexems) && (*lexems) != last_lexems && \
		!((t_token*)(*lexems)->data)->blank)
		concat_var(&(*lexems), (t_token*)(*lexems)->data, \
			(t_token*)(*lexems)->prev->data, 0);
}

static void	replace_var(char last_blank, t_list **lexems, t_sh *sh)
{
	char	*to_free;
	t_token	*token;
	char	*var;

	token = (t_token*)(*lexems)->data;
	if (TYPE == VAR_WORD)
	{
		TYPE = WORD;
		to_free = VAL;
		if (!(var = find_env(sh->env, to_free)))
			if (!(var = find_env(sh->shell_var, to_free)))
				var = "";
		var = ft_strlen(VAL) + ft_strlen(var) > MAX_CMD ? "" : var;
		ft_strlen(VAL) + ft_strlen(var) > MAX_CMD ? \
			ft_fputstr("Environment variable too large\n", 2) : NULL;
		VAL = ft_strdup(var);
		free(to_free);
	}
	if ((*lexems)->prev && !last_blank)
		concat_var(lexems, token, (t_token*)(*lexems)->prev->data, 1);
	else if ((*lexems)->next && !((t_token*)(*lexems)->next->data)->blank)
		concat_var(&(*lexems)->next, (t_token*)(*lexems)->next->data, token, 0);
	if ((*lexems)->prev && ((t_token*)(*lexems)->prev->data)->type == EQUAL)
		TYPE = VAR_WORD;
}

void		manage_var_op(t_sh *sh, t_list **tmp, t_list **cmd_tokens,
					t_token *token)
{
	char	last_blank;

	last_blank = BLK;
	if (*tmp == *cmd_tokens)
		*cmd_tokens = (*cmd_tokens)->next;
	ft_pop_node(tmp, (void*)&clear_lexems);
	if (*tmp)
		replace_var(last_blank, tmp, sh);
}

void		replace_tild(t_token *token, t_env *env)
{
	char	*to_free;

	if (TYPE == TILD)
	{
		TYPE = WORD;
		to_free = VAL;
		VAL = ft_strjoin(find_env(env, "HOME"), VAL + 1);
		free(to_free);
	}
	else if (TYPE == TILD_EXPR)
	{
		TYPE = EXPR;
		to_free = VAL;
		VAL = ft_strjoin(find_env(env, "HOME"), VAL + 1);
		free(to_free);
	}
	else if (TYPE == TILD_VAR_WORD)
	{
		TYPE = VAR_WORD;
		to_free = VAL;
		VAL = ft_strjoin(find_env(env, "HOME"), VAL + 1);
		free(to_free);
	}
}
