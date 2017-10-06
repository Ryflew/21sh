#include "tosh.h"

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

static t_token	*par_rule(t_sh *sh)
{
	t_token *token;

	eat(sh, LPAR);
	subshell(sh, LPAR);
	if (!sh->current_token || eat(sh, RPAR) == -1)
	{
		ft_putendl("parse error: miss close par `)`");
		return ((void*)-1);
	}
	if (!(token = text_rules(sh)))
		return (new_token(NULL, NONE, ""));
	return (token);
}

static t_token	*bqt_rule(t_sh *sh)
{
	t_token *token;
	char	concat = 0;
	t_token	*prev_token;

	prev_token = NULL;
	if (sh->lexer->lexems->prev)
		prev_token = (t_token*)sh->lexer->lexems->prev->data;
	if (sh->current_token->type == BQT)
		eat(sh, BQT);
	else
	{
		eat(sh, BQT_C);
		concat = 1;
	}
	if (!(subshell(sh, BQT)))
		return ((void*)-1);
	if (!sh->current_token || eat(sh, EBQT) == -1)
	{
		ft_putendl("parse error: miss close backquote '`'");
		return ((void*)-1);
	}
	if (!(token = text_rules(sh)))
		return (new_token(NULL, NONE, ""));
	else if (concat && prev_token && (prev_token->type == WORD || prev_token->type == NUM || prev_token->type == TILD_VAR_WORD || prev_token->type == TILD || prev_token->type == VAR_WORD))
	{
		free_join(&prev_token->value, VAL);
		token = prev_token;
	}
	else if (prev_token && prev_token->type == EQUAL)
		token->type = VAR_WORD;
	return (token);
}

t_token			*text_rules(t_sh *sh)
{
	t_token *token;

	if (!sh->current_token)
		return (NULL);
	token = sh->current_token;
	if (sh->current_token->type == WORD)
	{
		eat(sh, WORD);
		if (sh->current_token && sh->current_token->type == BQT_C)
			return (text_rules(sh));
	}
	else if (sh->current_token->type == NUM)
	{
		eat(sh, NUM);
		if (sh->current_token && sh->current_token->type == BQT_C)
		return (text_rules(sh));
	}
	else if (sh->current_token->type == END_EXPR)
		eat(sh, END_EXPR);
	else if (sh->current_token->type == S_WILDCARD)
		eat(sh, S_WILDCARD);
	else if (sh->current_token->type == Q_WILDCARD)
		eat(sh, Q_WILDCARD);
	else if (sh->current_token->type == E_WILDCARD)
		eat(sh, E_WILDCARD);
	else if (sh->current_token->type == LBKT)
		eat(sh, LBKT);
	else if (sh->current_token->type == RBKT)
		eat(sh, RBKT);
	else if (sh->current_token->type == LBRC)
		eat(sh, LBRC);
	else if (sh->current_token->type == RBRC)
		eat(sh, RBRC);
	else if (sh->current_token->type == COM)
		eat(sh, COM);
	else if (sh->current_token->type == EXPR)
		eat(sh, EXPR);
	else if (sh->current_token->type == BKT_EXPR)
		eat(sh, BKT_EXPR);
	else if (sh->current_token->type == TILD_EXPR)
		eat(sh, TILD_EXPR);
	else if (sh->current_token->type == TILD)
	{
		eat(sh, TILD);
		if (sh->current_token && sh->current_token->type == BQT_C)
			return (text_rules(sh));
	}
	else if (sh->current_token->type == VAR_OP)
		eat(sh, VAR_OP);
	else if (sh->current_token->type == VAR_OP_C)
		eat(sh, VAR_OP_C);
	else if (sh->current_token->type == VAR_WORD)
	{
		eat(sh, VAR_WORD);
		if (sh->current_token && sh->current_token->type == BQT_C)
			return (text_rules(sh));
	}
	else if (sh->current_token->type == TILD_VAR_WORD)
	{
		eat(sh, TILD_VAR_WORD);
		if (sh->current_token && sh->current_token->type == BQT_C)
			return (text_rules(sh));
	}
	else if (sh->current_token->type == EQUAL)
		eat(sh, EQUAL);
	else if (TYPE == BQT || TYPE == BQT_C)
		return (bqt_rule(sh));
	else if (TYPE == LPAR)
		return (par_rule(sh));
	else
		return (NULL);
	return (token);
}
