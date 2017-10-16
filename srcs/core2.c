/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/10 20:24:01 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/10/10 20:24:01 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

static char	exec_cmds(t_tree *node, t_env **env, t_sh *shell)
{
	char	ret;

	if ((ret = is_writable_builtins(node->cmds)) != 1)
		run_builtins(node, env, shell);
	else if (!(ret = is_builtins(node->cmds)))
		ret = go_builtins(node->cmds, env, shell);
	else
		ret = run_binary(node, *env, shell);
	return (ret);
}

static char	**create_cmds_with_tokens(t_list *lexems)
{
	t_token	*token;
	char	**cmds;
	int		nb_cmds;
	int		i;

	nb_cmds = ft_list_size(lexems);
	if (!(cmds = (char**)malloc(sizeof(char*) * (nb_cmds + 1))))
		ft_exiterror("ERROR: malloc failure", 0);
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

char		**get_cmds(t_list **cmd_tokens, t_sh *sh)
{
	t_list	*tmp;
	t_token	*token;

	tmp = *cmd_tokens;
	while (tmp)
	{
		token = (t_token*)tmp->data;
		if (TYPE == VAR_OP || TYPE == VAR_OP_C)
			manage_var_op(sh, &tmp, cmd_tokens, token);
		else if (TYPE == TILD || TYPE == TILD_VAR_WORD)
			replace_tild(token, sh->env);
		NEXT(tmp);
	}
	if (!sh->lexer->her)
	{
		glob(cmd_tokens);
		if (!is_env_var_to_add(cmd_tokens, *cmd_tokens, 0))
			return (0);
		concat_unused_add_var(*cmd_tokens);
	}
	return (create_cmds_with_tokens(*cmd_tokens));
}

static void	replace_subshell(t_sh *sh, t_list **cmd_tokens, char is_cmd)
{
	t_list	*lexems;
	t_token	*token;

	lexems = *cmd_tokens;
	while (lexems)
	{
		token = (t_token*)lexems->data;
		//ft_putstr("val = ");
		//ft_putendl(VAL);		
		if (TYPE == BQT || TYPE == BQT_C)
			bqt_rule(sh, &lexems, cmd_tokens, is_cmd);
		else if (TYPE == LPAR && is_cmd)
			par_rule(sh, &lexems, cmd_tokens);
		else
			NEXT(lexems);
	}
}

char		manage_cmds(t_tree *node, t_sh *sh, char is_cmd)
{
	replace_subshell(sh, &node->cmd_tokens, is_cmd);
	if (node->tmp_env)
		env_command(node->tmp_env, sh->env);
	if (!(node->cmds = get_cmds(&node->cmd_tokens, sh)))
		return (0);
	if (!is_cmd)
		return (1);
	if (find_env(sh->env, "PATH"))
		try_add_hashtab(node, sh);
	treat_history_cmd(node);
	/*for (int i = 0; node->cmds[i]; i++)
	{
		ft_putstr("tab = ");
		ft_putendl(node->cmds[i]);
	}*/
	return (exec_cmds(node, &(sh->env), sh));
}
