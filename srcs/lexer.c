#include "tosh.h"

static void		init_lexer(t_lexer *lexer)
{
	lexer->lexems = NULL;
	lexer->brc = 0;
	lexer->bkt = 0;
	lexer->bqt = 0;
	lexer->blank = 0;
	lexer->red = 0;
}

char	isnt_here_or_bqt(t_lexer *lexer)
{
	return (!lexer->her || lexer->bqt);
}

static t_token	*get_next_token(t_lexer *lexer, t_token *last_token)
{
	t_token *token;

	token = NULL;
	while (*lexer->line)
	{
		token = NULL;
		if (ft_isblank(*lexer->line))
		{
			while (*lexer->line && ft_isblank(*lexer->line))
				++lexer->line;
			lexer->blank = 1;
			if (isnt_here_or_bqt(lexer) && last_token && (is_glob_token(last_token->type) || last_token->type == EXPR || last_token->type == TILD_EXPR || last_token->type == BKT_EXPR || last_token->type == COM))
				return (new_token(lexer, END_EXPR, ""));
		}
		else
		{
			//ft_putendl("before lex");
			token = identify_token(lexer, last_token);
			//ft_putendl(VAL);	
			lexer->blank = 0;
		}
		if (token)
			return (token);
	}
	if (isnt_here_or_bqt(lexer) && last_token && (is_glob_token(last_token->type) || last_token->type == EXPR || last_token->type == TILD_EXPR || last_token->type == BKT_EXPR || last_token->type == COM))
		return (new_token(lexer, END_EXPR, ""));
	return (token);
}

void			get_lexems(t_sh *sh)
{
	t_token *token;

	init_lexer(sh->lexer);
	token = NULL;
	while ((token = get_next_token(sh->lexer, token)))
		ft_node_push_back(&(sh->lexer->lexems), token);
	ft_putendl("after lex");	
	t_list *tmp = sh->lexer->lexems;
	while (tmp)
	{
		ft_putnbr(((t_token*)(tmp->data))->type);
		ft_putendl(" <-- type");
		ft_putendl(((t_token*)(tmp->data))->value);
		tmp = tmp->next;
	}
}
