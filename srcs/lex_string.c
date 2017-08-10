#include "tosh.h"

int			is_string_op(int c)
{
	if (c == '\'' || c == '"' || c == '`')
		return (1);
	return (0);
}

static int	is_operator(int c, int c2)
{
	if (c == '>' || c == '<' || (c == '&' && c2 == '&') || c == '|' || \
		c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || \
		c == '}' || c == ';')
		return (1);
	return (0);
}

static void	lex_number2(t_lexer *lexer, int i, e_token *type, char *string_op)
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
}

t_token		*lex_word(t_lexer *lexer)
{
	int		i;
	t_token	*token;

	i = 0;
	token = NULL;
	while (((lexer->line)[i] && (lexer->string_operator && (lexer->line[i]) !=\
		lexer->string_operator)) || (!lexer->string_operator && \
		!is_operator((lexer->line)[i], (lexer->line)[i + 1]) \
		&& !ft_isblank((lexer->line)[i])))
		++i;
	token = new_token(lexer, WORD, clear_quot(remove_useless_space(\
		ft_strsub(lexer->line, 0, i), -1, -1, 0), lexer->string_operator));
	return (token);
}
