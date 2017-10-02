#include "tosh.h"

char	exec_cmds(t_tree *node, t_env **env, t_sh *shell)
{
	char	ret;

	if ((ret = is_writable_builtins(node->cmds[0])) != 1)
		run_builtins(node, env, shell);
	else if (!(ret = is_builtins(node->cmds)))
		ret = go_builtins(node->cmds, env, shell);
	else
		ret = run_binary(node, *env, shell);
	return (ret);
}

void	add_to_history(t_sh *shell, char *command)
{
	int		i;
	char	*tmp;

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
		if (TYPE == WORD && i < nb_cmds)
			cmds[i++] = token->value;
		lexems = lexems->next;
	}
	return (cmds);
}

char	manage_cmds(t_tree *node, t_sh *sh)
{
	t_list	*tmp;
	t_token	*token;

	/*ft_putendl("BEFORE");
	ft_putendl("START-----------------------------------------");
	t_list *tmp2 = sh->begin;
	while (tmp2)
	{
		token = (t_token*)tmp2->data;
			ft_putendl(token->value);
			ft_putnbr(token->type);
			ft_putendl("");
		tmp2 = tmp2->next;
	}
	ft_putendl("-----------------------------------------END");*/
	tmp = node->cmd_tokens;
	while (tmp)
	{
		token = (t_token*)tmp->data;
		if (TYPE == VAR_OP)
		{
			ft_pop_node(&tmp, NULL);
			if (tmp)
				replace_var(tmp->data, sh->env);
		}
		else if (TYPE == TILD)
			replace_tild(token, sh->env);
		tmp = tmp->next;
	}
	glob(&node->cmd_tokens);
	node->cmds = create_cmds_with_tokens(node->cmd_tokens);
	return (exec_cmds(node, &(sh->env), sh));
}