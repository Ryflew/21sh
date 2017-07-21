#include "21sh.h"

static char		fill_fd(t_sh *sh, int *fd, int fd0, int fd1)
{
	fd[0] = fd0;
	eat(sh, FRED);
	fd[1] = fd1;
	eat(sh, FD);
	return (1);
}

char			aggregation_rules(t_sh *sh, int *fd)
{
	if (!sh->current_token)
		return (0);
	if (sh->current_token->type == FD && sh->lexer->lexems->next && \
		((t_token*)sh->lexer->lexems->next->data)->type == FRED && \
		sh->lexer->lexems->next->next && \
		((t_token*)sh->lexer->lexems->next->next->data)->type == FD)
	{
		fd[0] = ft_atoi(sh->current_token->value);
		eat(sh, FD);
		eat(sh, FRED);
		fd[1] = ft_atoi(sh->current_token->value);
		eat(sh, FD);
		return (1);
	}
	else if (sh->current_token->type == FRED && sh->lexer->lexems->next && \
		((t_token*)sh->lexer->lexems->next->data)->type == FD)
		return fill_fd(sh, fd, 1, ft_atoi(sh->current_token->value));
	else if (sh->current_token->type == FD && sh->lexer->lexems->next && \
		((t_token*)sh->lexer->lexems->next->data)->type == FRED)
		return fill_fd(sh, fd, ft_atoi(sh->current_token->value), -1);
	return (0);
}

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

static t_token	*str_rules(t_sh *sh)
{
	t_token *old_token;
	t_token *token;

	if (!sh->current_token)
		return (NULL);
	token = NULL;
	if (is_string_op(*sh->current_token->value))
	{
		old_token = sh->current_token;
		eat(sh, sh->current_token->type);
		token = sh->current_token;
		eat(sh, WORD);
		if (eat(sh, old_token->type) == -1)
			return ((void*)-1);
	}
	return (token);
}

t_token			*text_rules(t_sh *sh)
{
	t_token *token;

	if (!sh->current_token)
		return (NULL);
	token = sh->current_token;
	if (token->type == WORD)
		eat(sh, WORD);
	else if (sh->current_token->type == NUM)
		eat(sh, NUM);
	else
		token = str_rules(sh);
	return (token);
}
