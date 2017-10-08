#include "tosh.h"

static void	free_lexems(t_sh *sh, t_list **lexems, t_list **cmd_tokens)
{
	if (*lexems == *cmd_tokens)
		*cmd_tokens = (*lexems)->next;
	if (!sh->lexer->her)
		ft_pop_node(lexems, NULL);
	else
		ft_pop_node(lexems, (void*)&clear_lexems);
}

static void	concat_unused_add_var(t_list *cmd_tokens, t_sh *sh)
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
		if (tmp->next && TYPE == VAR_WORD && (next_token->type == EQUAL
			|| next_token->type == VAR_WORD))
		{
			free_join(&VAL, next_token->value);
			free_lexems(sh, &tmp->next, &cmd_tokens);
		}
		else
		{
			if (TYPE == VAR_WORD)
				TYPE = WORD;
			NEXT(tmp);
		}
	}
}

static char	is_env_var_to_add(t_list **cmd_tokens, t_sh *sh)
{
	t_list	*tmp;
	t_token	*token;
	char	ret;
	char	*var_content;

	tmp = *cmd_tokens;
	while (tmp && !ret)
	{
		token = (t_token*)tmp->data;
		if (TYPE != EQUAL && TYPE != VAR_WORD)
			ret = 1;
		else if (TYPE == EQUAL && tmp->prev && ((t_token*)tmp->prev->data)->type == VAR_WORD)
		{
				var_content = "";
				if (tmp->next && ((t_token*)tmp->next->data)->type == VAR_WORD)
					var_content = ((t_token*)tmp->next->data)->value;
				add_var_to_shell(((t_token*)tmp->prev->data)->value, \
				var_content, &(get_shell()->shell_var));
				free_lexems(sh, &tmp->prev, cmd_tokens);
				free_lexems(sh, &tmp, cmd_tokens);
				if (tmp && ((t_token*)tmp->data)->type == VAR_WORD)
					free_lexems(sh, &tmp, cmd_tokens);
		}
		else
			NEXT(tmp);
	}
	return (ret);
}

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
		if (i < nb_cmds)
			cmds[i++] = ft_strdup(token->value);
		NEXT(lexems);
	}
	return (cmds);
}

char	**get_cmds(t_list **cmd_tokens, t_sh *sh)
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
		glob(cmd_tokens);
	if (!is_env_var_to_add(cmd_tokens, sh))
		return (0);
	concat_unused_add_var(*cmd_tokens, sh);
	return (create_cmds_with_tokens(*cmd_tokens));
}

char	manage_cmds(t_tree *node, t_sh *sh)
{
	if (!(node->cmds = get_cmds(&node->cmd_tokens, sh)))
		return (0);
	if (find_env(sh->env, "PATH"))
		try_add_hashtab(node, sh);
	treat_history_cmd(node);
	return (exec_cmds(node, &(sh->env), sh));
}
