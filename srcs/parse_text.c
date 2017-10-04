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
	if (eat(sh, RPAR) == -1)
		return ((void*)-1);
	if (!(token = text_rules(sh)))
		return (new_token(NULL, NONE, ""));
	return (token);
}

static t_token	*bqt_rule(t_sh *sh)
{
	t_token *token;

	eat(sh, BQT);
	if (!(subshell(sh, BQT)))
		return ((void*)-1);
	if (eat(sh, EBQT) == -1)
		return ((void*)-1);
	if (!(token = text_rules(sh)))
		return (new_token(NULL, NONE, ""));
	return (token);
}

t_token			*text_rules(t_sh *sh)
{
	t_token *token;

	if (!sh->current_token)
		return (NULL);
	token = sh->current_token;
	if (sh->current_token->type == WORD)
		eat(sh, WORD);
	else if (sh->current_token->type == NUM)
		eat(sh, NUM);
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
		eat(sh, TILD);
	else if (sh->current_token->type == VAR_OP)
		eat(sh, VAR_OP);
	else if (sh->current_token->type == VAR_WORD)
		eat(sh, VAR_WORD);
	else if (TYPE == BQT)
		return (bqt_rule(sh));
	else if (TYPE == LPAR)
		return (par_rule(sh));
	else
		return (NULL);
	return (token);
}
