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

// METTRE DANS UN AUTRE FICHIER
void	add_to_history(t_sh *shell, char *command)
{
	int		i;
	char	*tmp;

	if (!command || !*command)
		return ;
	if (!ft_strcont(command, "\n"))
		add_line(shell, command);
	else
	{
		i = -1;
		while (command[++i] && command[i] != '\n')
			;
		tmp = ft_strsub(command, 0, i);
		add_line(shell, tmp);
		free(tmp);
	}
	shell->history_pos = -1;
}

char	**create_cmds_with_tokens(t_list *lexems)
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
		lexems = lexems->next;
	}
	return (cmds);
}

char	is_env_var_to_add(t_list *cmd_tokens)
{
	t_list	*tmp;
	t_token	*token;

	tmp = cmd_tokens;
	while (tmp)
	{
		token = (t_token*)tmp->data;
		if (TYPE == EQUAL)
		{
			if (tmp->prev && tmp->prev->prev && !ft_strcmp(((t_token*)tmp->prev->prev->data)->value, "export"))
				;// VICTOR doit implémenter export et setenv ici mais aussi yolo=ok; export yolo
			return (0);
		}
		tmp = tmp->next;
	}
	return (1);
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
		tmp = tmp->next;
	}
	if (!sh->lexer->her)
		glob(cmd_tokens);
	return (create_cmds_with_tokens(*cmd_tokens));
}

char	manage_cmds(t_tree *node, t_sh *sh)
{
	node->cmds = get_cmds(&node->cmd_tokens, sh);
	if (find_env(sh->env, "PATH"))
		try_add_hashtab(node, sh);
	treat_history_cmd(node);
	if (!is_env_var_to_add(node->cmd_tokens))
		return (0);
	return (exec_cmds(node, &(sh->env), sh));
}