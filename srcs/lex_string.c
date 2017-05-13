#include "21sh.h"

static int is_operator(int c, int c2)
{
	if (c == '>' || c == '<' || (c == '&' && c2 == '&') || c == '|' || c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}' || c == ';')
		return (1);
	return (0);
}

int is_string_op(int c)
{
	if (c == '\'' || c == '"' || c == '`')
		return (1);
	return (0);
}

t_token *lex_str(t_lexer *lexer)
{
	int i;
	t_token *token;

	i = 0;
	token = NULL;
	//ft_putendl("STR");
	while ((lexer->line)[i] && (lexer->line)[i] != lexer->string_operator)
		++i;
	token = new_token(lexer, TEXT, ft_strsub(lexer->line, 0, i));
	return (token);
}

t_token *lex_number(t_lexer *lexer)
{
	int i;
	e_token type;
	t_token *token;
	char string_op;

	string_op = 0;
	type = NUM;
	token = NULL;
	i = -1;
	while (((lexer->line)[++i] && (lexer->line)[i] != ' ' && !is_operator((lexer->line)[i], (lexer->line)[i + 1])) || (string_op && (lexer->line)[i]))
	{
		if (!ft_isdigit((lexer->line)[i]))
			type = WORD;
		if (is_string_op((lexer->line)[i]))
		{
			if (!string_op)
				string_op = (lexer->line)[i];
			else
				string_op = 0;
		}
	}
	if (lexer->red || (type == NUM && (lexer->line)[i] && ((lexer->line)[i] == '<' || (lexer->line)[i] == '>'))) // && (lexer->line)[i + 1] == '&'))
		type = FD;
	token = new_token(lexer, type, clear_quot(ft_strsub(lexer->line, 0, i)));
	//token->value = clear_quot(token->value);
	return (token);
}

t_token *lex_word(t_lexer *lexer)
{
	int i;
	t_token *token;
	char string_op;

	string_op = 0;
	i = 0;
	token = NULL;
	//ft_putendl("WORD");
	while (((lexer->line)[i] && !is_operator((lexer->line)[i], (lexer->line)[i + 1]) && !ft_isblank((lexer->line)[i])) || (string_op && (lexer->line)[i]))
	{
		if (is_string_op((lexer->line)[i]))
		{
			if (!string_op)
				string_op = (lexer->line)[i];
			else
				string_op = 0;
		}
		++i;
	}
	token = new_token(lexer, WORD, clear_quot(remove_useless_space(ft_strsub(lexer->line, 0, i))));
	//token->value = clear_quot(token->value);
	return (token);
}