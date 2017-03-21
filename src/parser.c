#include "21sh.h"

t_token	*eat(e_token current_token, e_token token, t_lexer *lexer)
{
	if (current_token == token)
		return (get_next_token(lexer));
	else
	{
		syntax_error();
		return (NULL);
	}
}
