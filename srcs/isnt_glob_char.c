#include "tosh.h"

static char	isnt_s_wildcard(t_lexer *lexer, char c)
{
	return (c == '*' && lexer->bkt) ? 1 : 0;
}

static char	isnt_q_wildcard(t_lexer *lexer, char c)
{
	return (c == '?' && lexer->bkt) ? 1 : 0;
}

static char	isnt_comma(t_lexer *lexer, char c)
{
	return (c == ',' && (!lexer->brc || lexer->bkt)) ? 1 : 0;
}

char		isnt_glob_char(t_lexer *lx, char c, int i)
{
	if (isnt_lbrc(lx, c) || isnt_rbrc(lx, c) || isnt_comma(lx, c) || isnt_lbkt(lx, c) \
	|| isnt_rbkt(lx, c, i) || isnt_s_wildcard(lx, c) || isnt_q_wildcard(lx, c))
		return (1);
	return (0);
}