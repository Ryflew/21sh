#include "21sh.h"

char			eat(t_sh *sh, e_token token)
{
	if (sh->current_token->type == token)
	{
		sh->lexer->lexems = sh->lexer->lexems->next;
		if (sh->lexer->lexems)
			sh->current_token = sh->lexer->lexems->data;
		else
		sh->current_token = NULL;
	}
	else
		return (-1);
	return (0);
}

static t_tree	*pipe_rules(t_sh *sh, t_tree *left)
{
	t_token	*token;
	t_tree	*new_node;

	new_node = NULL;
	while (sh->current_token && sh->current_token->type == PIPE)
	{
		if (!left->right && left->token && left->token->type != SCL)
			return ((void*)-1);
		token = sh->current_token;
		eat(sh, PIPE);
		if ((new_node = create_node(left, token, NULL, cmd_rules(sh))))
			left = new_node;
	}
	return (new_node);
}

static t_tree	*cmd_with_op_rules(t_sh *sh)
{
	t_tree	*left;
	t_tree	*tmp;

	left = NULL;
	if (!sh->current_token || ((!(left = cmd_rules(sh)) && sh->current_token->type != CHEVF &&
		sh->current_token->type != DCHEVF && sh->current_token->type != CHEVB \
		&& sh->current_token->type != DCHEVB) || left == (void*)-1))
		return (left);
	while ((left || (sh->current_token->type == CHEVF || \
		sh->current_token->type == DCHEVF || sh->current_token->type == CHEVB \
		|| sh->current_token->type == DCHEVB)) && left != (void*)-1)
	{
		if ((tmp = pipe_rules(sh, left)) && tmp != (void*)-1)
			left = tmp;
		else if ((tmp = redirection_rules(sh, left)) && tmp != (void*)-1)
			left = tmp;
		if (!tmp)
			return (left);
		else if (tmp == (void*)-1)
			return (tmp);
	}
	return (left);
}

static t_tree	*condition_operators_rules(t_sh *sh)
{
	t_tree	*left;
	t_tree	*right;
	t_token	*token;

	if (!(left = cmd_with_op_rules(sh)) || left == (void*)-1)
		return (left);
	while (sh->current_token && (sh->current_token->type == OR || sh->current_token->type == AND))
	{
		token = sh->current_token;
		if (token->type == OR)
			eat(sh, OR);
		else if (token->type == AND)
			eat(sh, AND);
		if ((right = cmd_with_op_rules(sh)) != (void*)-1)
			left = create_node(left, token, NULL, right);
	}
	return (left);
}

static char		**parse_env_cmds(t_sh *sh)
{
	int		size;
	t_list	*tmp;
	int		i;
	char	**tmp_env;
	t_token	*token;

	tmp = sh->lexer->lexems;
	if (ft_strcmp(sh->current_token->value, "env") || !tmp->next)
		return (NULL);
	tmp = tmp->next;
	token = tmp->data;
	size = 1;
	while (tmp && (!ft_strcmp(token->value, "-i") || !ft_strcmp(token->value, "-u") || !ft_strcmp(token->value, "env") || ft_strchr(token->value, '=')))
	{
		++size;
		tmp = tmp->next;
		if (tmp)
			token = tmp->data;		
	}
	if (!token)
	{
		sh->current_token = NULL;
		return (NULL);
	}
	if (ft_strcmp(token->value, "env"))
		++size;
	tmp_env = (char**)malloc(sizeof(char*) * size);
	tmp_env[size - 1] = NULL;
	i = 0;
	--size;
	while (i++ < size)
	{
		token = text_rules(sh);		
		tmp_env[i] = token->value;
		free(token);
	}
	return (tmp_env);
}

t_tree			*commands_line_rules(t_sh *sh)
{
	t_tree	*left;
	t_tree	*right;
	t_token	*token;
	char	**tmp_env;

	while (sh->current_token && sh->current_token->type == SCL)
		eat(sh, SCL);
	tmp_env = parse_env_cmds(sh);
	if (!(left = condition_operators_rules(sh)) || left == (void*)-1)
	{
		free(tmp_env);
		return (left);
	}
	left->tmp_env = tmp_env;
	while (sh->current_token && sh->current_token->type == SCL)
	{
		token = sh->current_token;
		while (sh->current_token && sh->current_token->type == SCL)
			eat(sh, SCL);
		tmp_env = parse_env_cmds(sh);
		if ((right = condition_operators_rules(sh)))
		{
			right->tmp_env = tmp_env;
			left = create_node(left, token, NULL, right);
		}
	}
	return (left);
}
