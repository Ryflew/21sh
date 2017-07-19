#include "21sh.h"

t_token	*str_rules(t_sh *sh)
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

t_token	*text_rules(t_sh *sh)
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
