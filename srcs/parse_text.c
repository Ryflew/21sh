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

static void	bqt_c_rule(t_sh *sh, e_token type, t_token **token)
{
	eat(sh, type);
	if (sh->current_token && sh->current_token->type == BQT_C)
		*token = text_rules(sh);
}

static void	text_rules3(t_sh *sh, t_token **token)
{
	if (sh->current_token->type == VAR_OP)
		eat(sh, VAR_OP);
	else if (sh->current_token->type == VAR_OP_C)
		eat(sh, VAR_OP_C);
	else if (sh->current_token->type == VAR_WORD)
		bqt_c_rule(sh, VAR_WORD, token);
	else if (sh->current_token->type == TILD_VAR_WORD)
		bqt_c_rule(sh, TILD_VAR_WORD, token);
	else if (sh->current_token->type == EQUAL)
		eat(sh, EQUAL);
	else if ((*token)->type == BQT || (*token)->type == BQT_C)
		*token = bqt_rule(sh);
	else if ((*token)->type == LPAR)
		*token = par_rule(sh);
	else
		*token = NULL;
}

static void	text_rules2(t_sh *sh, t_token **token)
{
	if (sh->current_token->type == S_WILDCARD)
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
	else
		text_rules3(sh, token);
}

t_token			*text_rules(t_sh *sh)
{
	t_token *token;

	if (!sh->current_token)
		return (NULL);
	token = sh->current_token;
	if (sh->current_token->type == WORD)
		bqt_c_rule(sh, WORD, &token);
	else if (sh->current_token->type == NUM)
		bqt_c_rule(sh, NUM, &token);	
	else if (sh->current_token->type == TILD)
		bqt_c_rule(sh, TILD, &token);	
	else if (sh->current_token->type == END_EXPR)
		eat(sh, END_EXPR);
	else
		text_rules2(sh, &token);
	return (token);
}
