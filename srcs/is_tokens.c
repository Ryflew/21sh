#include "tosh.h"

char			is_string_op(char c)
{
	if (c == '\'' || c == '"')
		return (1);
	return (0);
}

char	is_operator(char c, char c2)
{
	if (c == '>' || c == '<' || (c == '&' && c2 == '&') || c == '|' || \
		c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || \
		c == '}' || c == ';' || c == '*' || c == '?' || c == '`' | c == '$' || c == '=')
		return (1);
	return (0);
}

char    is_glob_token(e_token type)
{
    if (type == S_WILDCARD || type == Q_WILDCARD || type == E_WILDCARD
         || type == LBKT || type == RBKT || type == LBRC || type == RBRC)
         return (1);
    return (0);
}