#include "tosh.h"

char			is_string_op(char c)
{
	if (c == '\'' || c == '"' || c == '`')
		return (1);
	return (0);
}

static char	is_operator(char c, char c2)
{
	if (c == '>' || c == '<' || (c == '&' && c2 == '&') || c == '|' || \
		c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || \
		c == '}' || c == ';' || c == '*' || c == '?')
		return (1);
	return (0);
}

/*static void	lex_number2(t_lexer *lexer, int i, e_token *type, char *string_op)
{
	if (!ft_isdigit((lexer->line)[i]))
		*type = WORD;
	if (is_string_op((lexer->line)[i]))
	{
		if (!*string_op)
			*string_op = (lexer->line)[i];
		else
			*string_op = 0;
	}
}

t_token		*lex_number(t_lexer *lexer)
{
	int		i;
	e_token	type;
	t_token	*token;
	char	string_op;

	string_op = 0;
	type = NUM;
	token = NULL;
	i = -1;
	while (((lexer->line)[++i] && (lexer->line)[i] != ' ' && \
		!is_operator((lexer->line)[i], (lexer->line)[i + 1])) || \
		(string_op && (lexer->line)[i]))
		lex_number2(lexer, i, &type, &string_op);
	if (lexer->red || (type == NUM && (lexer->line)[i] && \
		((lexer->line)[i] == '<' || (lexer->line)[i] == '>')))
		type = FD;
	token = new_token(lexer, type, clear_quot(ft_strsub(lexer->line, 0, i),\
	lexer->string_operator));
	return (token);
}*/

static char	*get_string(t_lexer *lexer, char const *s, size_t len)
{
	char			*strf;
	size_t			i;
	char			first_string_op;
	size_t			new_len;
	char			bs;

	if (s)
	{
		first_string_op = lexer->string_operator;
		i = -1;
		new_len = 0;
		bs = 0;
		if (lexer->bs)
			bs = 1;
		while (++i < len)
		{
			if (!bs && is_string_op(s[i]) && !lexer->string_operator)
				lexer->string_operator = s[i];
			else if (s[i] == lexer->string_operator && !bs)
				lexer->string_operator = 0;
			else if (bs || (s[i] != lexer->string_operator && s[i] != '\\'))
				++new_len;
			if (s[i] == '\\' && lexer->string_operator != '\'' && !bs)
				bs = 1;
			else
				bs = 0;
		}
		strf = (char*)malloc(sizeof(char) * (new_len + 1));
		if (strf)
		{
			i = -1;
			new_len = 0;
			lexer->string_operator = first_string_op;
			bs = 0;
			if (lexer->bs)
				bs = 1;
			while (++i < len)
			{
				if (!bs && is_string_op(s[i]) && !lexer->string_operator)
					lexer->string_operator = s[i];
				else if (s[i] == lexer->string_operator && !bs)
					lexer->string_operator = 0;
				else if (bs || (s[i] != lexer->string_operator && s[i] != '\\'))
					strf[new_len++] = s[i];
				if (s[i] == '\\' && lexer->string_operator != '\'' && !bs)
					bs = 1;
				else
					bs = 0;
			}
			strf[new_len] = '\0';
			return (strf);
		}
	}
	return (NULL);
}

t_token		*lex_word(t_lexer *lexer, t_token *last_token)
{
	int		i;
	t_token	*token;
	e_token	type;
	char	bs;
	char	st_op;

	i = 0;
	token = NULL;
	type = NUM;
	bs = lexer->bs;
	st_op = lexer->string_operator;
	while ((lexer->line)[i] && (bs ||
		(!is_operator((lexer->line)[i], (lexer->line)[i + 1]) \
		&& !ft_isblank((lexer->line)[i]) && ((lexer->line)[i] != ',' || !lexer->brc || lexer->bkt))
		|| ((lexer->line)[i] == ']' && !lexer->bkt) || ((lexer->line)[i] == '}' && !lexer->brc)
		|| ((lexer->line)[i] == '{' && lexer->bkt) || ((lexer->line)[i] == '}' && lexer->bkt) || st_op))
	{
		if (type != WORD && !ft_isdigit((lexer->line)[i]))
			type = WORD;
		if (is_string_op((lexer->line)[i]) && !st_op)
			st_op = (lexer->line)[i];
		else if ((lexer->line)[i] == st_op)
			st_op = 0;
		if ((lexer->line)[i] == '\\' && st_op != '\'' && !bs)
			bs = 1;
		else
			bs = 0;
		++i;
	}
	if (type == NUM && (last_token->type == FRED || last_token->type == BRED || ((lexer->line)[i] && \
	((lexer->line)[i] == '<' || (lexer->line)[i] == '>'))))
		type = FD;
	else if (*lexer->line == '~' && (lexer->blank || !last_token || last_token->type == LBRC || last_token->type == COM) && (ft_isblank(*(lexer->line + 1)) || !*(lexer->line + 1) || *(lexer->line + 1) == '/' || *(lexer->line + 1) == '}' || *(lexer->line + 1) == ',') && !st_op)
		type = TILD;
	if (last_token && is_glob_token(last_token->type) && !lexer->blank)
	{
		if (lexer->bkt)
			type = BKT_EXPR;
		else if (type == TILD)
			type = TILD_EXPR;
		else
			type = EXPR;
	}
	token = new_token(lexer, type, get_string(lexer, lexer->line, i));
	lexer->line += i - ft_strlen(token->value);
	lexer->string_operator = 0;
	lexer->bs = 0;
	return (token);
}
