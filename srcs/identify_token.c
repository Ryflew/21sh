#include "21sh.h"

void		manage_string_op(t_lexer *lexer)
{
	if (!lexer->string_operator)
		lexer->string_operator = *lexer->line;
	else if (*lexer->line == lexer->string_operator)
		lexer->string_operator = 0;
	if (*lexer->line != '`')
		++lexer->line;
}

static void	find_token2(t_lexer *lexer, t_token **token)
{
	if (*lexer->line == '(')
		*token = new_token(lexer, LPAR, "(");
	else if (*lexer->line == ')')
		*token = new_token(lexer, RPAR, ")");
	else if (*lexer->line == '[')
		*token = new_token(lexer, LBKT, "[");
	else if (*lexer->line == ']')
		*token = new_token(lexer, RBKT, "]");
	else if (*lexer->line == '{')
		*token = new_token(lexer, LBRC, "{");
	else if (*lexer->line == '}')
		*token = new_token(lexer, RBRC, "}");
	else if (*lexer->line == ';')
		*token = new_token(lexer, SCL, ";");
	else if (ft_isdigit(*lexer->line))
		*token = lex_number(lexer);
	else if (*lexer->line)
		*token = lex_word(lexer);
}

t_token	    *find_token(t_lexer *lexer)
{
	t_token *token;

	token = NULL;
	if (*lexer->line == '>' && *(lexer->line + 1) == '>')
		token = new_token(lexer, DCHEVF, ">>");
	else if (*lexer->line == '<' && *(lexer->line + 1) == '<')
		token = new_token(lexer, DCHEVB, "<<");
	else if (*lexer->line == '>' && *(lexer->line + 1) == '&')
		token = new_token(lexer, FRED, ">&");
	else if (*lexer->line == '>')
		token = new_token(lexer, CHEVF, ">");
	else if (*lexer->line == '<')
		token = new_token(lexer, CHEVB, "<");
	else if (*lexer->line == '&' && *(lexer->line + 1) == '&')
		token = new_token(lexer, AND, "&&");
	else if (*lexer->line == '|' && *(lexer->line + 1) == '|')
		token = new_token(lexer, OR, "||");
	else if (*lexer->line == '|')
		token = new_token(lexer, PIPE, "|");
	else
		find_token2(lexer, &token);
	return (token);
}