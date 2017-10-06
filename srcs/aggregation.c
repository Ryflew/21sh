#include "tosh.h"

static char fill_fd(t_sh *sh, int *left_fd, int *right_fd, e_token type)
{
	*left_fd = ft_atoi(sh->current_token->value);
	eat(sh, FD);
	eat(sh, type);
	if (sh->current_token->type == FD)
	{
		*right_fd = ft_atoi(sh->current_token->value);
		eat(sh, FD);
	}
	else
	{
		*right_fd = -2;
		eat(sh, CLOSE_FD);
	}
	return (1);
}

static char	fred_bred_rules(t_sh *sh, int *left_fd, int *right_fd, 
			e_token type)
{
	if (sh->current_token->type == FD && sh->lexer->lexems->next && \
		((t_token*)sh->lexer->lexems->next->data)->type == type && \
		sh->lexer->lexems->next->next && \
		((t_token*)sh->lexer->lexems->next->next->data)->type == FD)
		return (fill_fd(sh, left_fd, right_fd, type));
	else if (sh->current_token->type == type && sh->lexer->lexems->next && \
		((t_token*)sh->lexer->lexems->next->data)->type == FD)
	{
        *left_fd = (type == BRED) ? 0 : 1;
		eat(sh, type);
		*right_fd = ft_atoi(sh->current_token->value);
		eat(sh, FD);
		return (1);
	}
	else if (sh->current_token->type == FD && sh->lexer->lexems->next && \
		((t_token*)sh->lexer->lexems->next->data)->type == type && \
		sh->lexer->lexems->next->next && \
		((t_token*)sh->lexer->lexems->next->next->data)->type == CLOSE_FD)
		return (fill_fd(sh, left_fd, right_fd, type));
	else if (sh->current_token->type == FD && sh->lexer->lexems->next && \
		((t_token*)sh->lexer->lexems->next->data)->type == type)
	{
		eat(sh, FD);
		eat(sh, type);
		return (-1);
	}
	return (0);
}

t_fd	*aggregation_rules(t_sh *sh)
{
	t_fd	*fd;
	char	ret;

	if (!(fd = (t_fd*)malloc(sizeof(t_fd))))
		ft_exiterror("Malloc failure in aggregation_rules !", 1);
	if (!sh->current_token)
	{
		free(fd);
		return (NULL);
	}
	if ((ret = fred_bred_rules(sh, &fd->to, &fd->from, BRED)) == -1)
	{
		free(fd);	
		return ((void*)-1);
	}
	else if (ret)
        fd->type = BRED;
    else if ((ret = fred_bred_rules(sh, &fd->from, &fd->to, FRED)) == -1)
	{
		free(fd);
		return ((void*)-1);
	}
	else if (ret)
        fd->type = FRED;
    else
    {
		free(fd);
		return (NULL);
	}
	return (fd);
}