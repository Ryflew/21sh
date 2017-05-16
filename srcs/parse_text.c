#include "21sh.h"

t_token *str_rules(t_sh *sh)
{
	t_token *old_token;
	t_token *token;

	if (!sh->current_token)
		return (NULL);
	token = NULL;
	//ft_putendl("step7-1-1");
	//	ft_putendl(sh->current_token->value);
	if (is_string_op(*sh->current_token->value))
	{
		//ft_putendl("step7-1-2");
		old_token = sh->current_token;
		eat(sh, sh->current_token->type);
		//ft_putendl("step7-1-3");
		//ft_putendl(sh->current_token->value);
		//ft_putnbr(sh->current_token->type);
		//ft_putnbr(TEXT);
		token = sh->current_token;
		eat(sh, TEXT);
		//ft_putendl("step7-1-4");
		//ft_putnbr(sh->current_token->type);
		//ft_putnbr(old_token->type);
		if (eat(sh, old_token->type) == -1)
			return ((void*)-1);
	}
	//ft_putendl("step7-1-5");
	return (token);
}

t_token *text_rules(t_sh *sh)
{
	t_token *token;

	if (!sh->current_token)
		return (NULL);
	token = sh->current_token;
	//ft_putendl("step7-1");
	//ft_putendl(token->value);
	if (token->type == WORD)
	{
		//ft_putendl("step7-2");
		eat(sh, WORD);
	}
	else if (sh->current_token->type == NUM)
		eat(sh, NUM);
	else
		token = str_rules(sh);
	//ft_putendl("step7-3 ret token");
	//if (token)
		//ft_putendl(token->value);
	return (token);
}