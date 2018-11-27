#include "tosh.h"

char	isnt_rbkt(t_lexer *lexer, char c, int i)
{
	return (c == ']' && (!lexer->bkt || !i)) ? 1 : 0;
}

char	isnt_lbkt(t_lexer *lexer, char c)
{
	return (c == '[' && lexer->bkt) ? 1 : 0;
}

char	isnt_rbrc(t_lexer *lexer, char c)
{
	return (c == '}' && !lexer->brc) ? 1 : 0;
}

char	isnt_lbrc(t_lexer *lexer, char c)
{
	return (c == '{' && lexer->bkt) ? 1 : 0;
}

char is_dot_or_slash(char c)
{
	return ((c == '.' || c == '/') ? 1 : 0);
}