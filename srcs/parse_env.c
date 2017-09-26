#include "tosh.h"

static char	**create_tmp_env(t_sh *sh, int size, t_token *token)
{
	char	**tmp_env;
	int		i;

	if (!token)
	{
		sh->current_token = NULL;
		return (NULL);
	}
	else if (size == 1)
	{
		token = text_rules(sh);
		return (NULL);
	}
	if (ft_strcmp(token->value, "env"))
		++size;
	tmp_env = (char**)malloc(sizeof(char*) * size);
	tmp_env[size - 1] = NULL;
	i = 0;
	--size;
	while (i < size)
	{
		token = text_rules(sh);
		tmp_env[i++] = token->value;	
	}
	return (tmp_env);
}

char		**parse_env_cmds(t_sh *sh)
{
	int		size;
	t_list	*tmp;
	t_token	*token;

	tmp = sh->lexer->lexems;
	if (!sh->current_token || ft_strcmp(sh->current_token->value, "env") || !tmp->next)
		return (NULL);
	tmp = tmp->next;
	token = tmp->data;
	size = 1;
	while (tmp && (!ft_strcmp(token->value, "-i") ||\
		!ft_strcmp(token->value, "-u") || !ft_strcmp(token->value, "env") ||\
		ft_strchr(token->value, '=')))
	{
		++size;
		tmp = tmp->next;
		if (tmp && !ft_strcmp(token->value, "-u"))
		{
			++size;
			tmp = tmp->next;
		}
		if (tmp)
			token = tmp->data;
	}
	return (create_tmp_env(sh, size, token));
}
