/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/10 20:24:01 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/12/06 17:42:23 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

static char	exec_cmds(t_tree *node, t_env **env, t_sh *shell)
{
	char	ret;

	if ((ret = is_writable_builtins(node->cmds)) != 1)
		ret = run_builtins(node, env, shell);
	else if (!(ret = is_builtins(node->cmds)))
		ret = go_builtins(node->cmds, env, shell);
	else
		ret = run_binary(node, *env, shell);
	if (ret)
	{
		shell->have_write_error = 1;
		shell->return_value = 1;
	}
	return (ret);
}

char		**create_cmds_with_tokens(t_list *lexems)
{
	t_token	*token;
	char	**cmds;
	int		nb_cmds;
	int		i;

	nb_cmds = ft_list_size(lexems);
	if (!(cmds = (char**)malloc(sizeof(char*) * (nb_cmds + 1))))
		ft_exiterror("Malloc failed", 1);
	cmds[nb_cmds] = NULL;
	i = 0;
	while (lexems)
	{
		token = (t_token*)lexems->data;
		if (i < nb_cmds && VAL)
			cmds[i++] = ft_strdup(VAL);
		NEXT(lexems);
	}
	return (cmds);
}

char	replace_tild_and_var_op(t_list **cmd_tokens, t_sh *sh)
{
	t_list	*tmp;
	t_token	*token;

	tmp = *cmd_tokens;
	while (tmp)
	{
		token = (t_token*)tmp->data;
		if (TYPE == VAR_OP)
			manage_var_op(sh, &tmp, cmd_tokens, token);
		else if (TYPE == TILD || TYPE == TILD_VAR_WORD || TYPE == TILD_EXPR)
			replace_tild(token, sh->env);
		else
			NEXT(tmp);
	}
	if (!sh->lexer->her)
	{
		glob(cmd_tokens);
		if (!is_env_var_to_add(cmd_tokens, *cmd_tokens, 0))
			return (0);
		concat_unused_add_var(*cmd_tokens);
	}
	return (1);
}

static char	replace_par_subshell(t_sh *sh, t_list **lexems, char is_cmd)
{
	t_token	*token;

	sh->lexer->lexems = *lexems;
	while (*lexems)
	{
		token = (t_token*)(*lexems)->data;
		if (TYPE == LPAR && is_cmd)
		{
			sh->ssbqt = RPAR;
			delete_first_subshell_lexems(&sh->lexer->lexems, lexems);
			if (*lexems)
			{
				subshell(sh, *lexems, RPAR, is_cmd);
				delete_subshell_lexems(&sh->lexer->lexems, lexems, LPAR, RPAR);
			}
			sh->ssbqt = 0;
		}
		else
			NEXT((*lexems));
	}
	*lexems = sh->lexer->lexems;
	return (0);
}

char		manage_cmds(t_tree *node, t_sh *sh, char is_cmd)
{
	if (node->tmp_env)
		env_command(node->tmp_env, sh->env);
	if (!replace_tild_and_var_op(&node->cmd_tokens, sh))
		return (0);
	replace_par_subshell(sh, &node->cmd_tokens, is_cmd);	
	if (!(node->cmds = create_cmds_with_tokens(node->cmd_tokens)))
		return (0);
	if (!is_cmd)
		return (1);
	if (find_env(sh->env, "PATH"))
		try_add_hashtab(node, sh);
	treat_history_cmd(node);
	return (exec_cmds(node, &(sh->env), sh));
}
