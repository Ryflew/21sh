#include "tosh.h"

char is_bs_or_bqt_or_par(t_lexer *lexer, char bs, char c)
{
	return (bs || (lexer->bqt && (c != '`' || lexer->bkt)) || (lexer->par && (c != ')' || lexer->bkt))) ? 1 : 0;
}

char isnt_var_val(t_token *last_token, char c)
{
	return (last_token && last_token->type == VAR_OP && !ft_isalnum(c)) ? 1 : 0;
}

char is_regular_char_in_st_op(char st_op, char c)
{
	return (st_op && ((c != '`' && c != '$') || st_op == '\'')) ? 1 : 0;
}

char isnt_var_op(char c1, char c2)
{
	return (c1 == '$' && !ft_isalnum(c2)) ? 1 : 0;
}

char isnt_equal(t_lexer *lexer, char c, int i)
{
	return (c == '=' && (!i || ft_isblank((lexer->line)[i - 1]))) ? 1 : 0;
}