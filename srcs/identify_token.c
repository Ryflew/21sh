#include "tosh.h"

void		manage_string_op(t_lexer *lexer)
{
	if (!lexer->string_operator)
		lexer->string_operator = *lexer->line;
	/*else if (*lexer->line == lexer->string_operator)
		lexer->string_operator = 0;*/
	if (*lexer->line != '`')
		++lexer->line;
}

static void	find_token2(t_lexer *lexer, t_token **token, t_token *last_token)
{
	if (!lexer->bs)
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
		else if (*lexer->line == '\\')
		{
			lexer->bs = 1;
			++lexer->line;
			return ;
		}
		else if (*lexer->line == '-' && last_token && (last_token->type == FRED || last_token->type == BRED))
			*token = new_token(lexer, CLOSE_FD, "-");
	}
	if (*lexer->line && !(*token))
		*token = lex_word(lexer, last_token);
}

t_token		*find_token(t_lexer *lexer, t_token *last_token)
{
	t_token *token;

	token = NULL;
	if (!lexer->bs)
	{
		if (*lexer->line == '>' && *(lexer->line + 1) == '>')
			token = new_token(lexer, DCHEVF, ">>");
		else if (*lexer->line == '<' && *(lexer->line + 1) == '<')
			token = new_token(lexer, DCHEVB, "<<");
		else if (*lexer->line == '>' && *(lexer->line + 1) == '&')
			token = new_token(lexer, FRED, ">&");
		else if (*lexer->line == '<' && *(lexer->line + 1) == '&')
			token = new_token(lexer, BRED, "<&");
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
	}
	if (!token)
		find_token2(lexer, &token, last_token);
	return (token);
}
