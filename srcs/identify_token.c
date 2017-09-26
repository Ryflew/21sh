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

char    is_glob_token(e_token type)
{
    if (type == S_WILDCARD || type == Q_WILDCARD || type == E_WILDCARD
         || type == LBKT || type == RBKT || type == LBRC || type == RBRC)
         return (1);
    return (0);
}

static void	find_token2(t_lexer *lexer, t_token **token, t_token *last_token)
{
	if (*lexer->line == '(')
		*token = new_token(lexer, LPAR, "(");
	else if (*lexer->line == ')')
		*token = new_token(lexer, RPAR, ")");
	else if (*lexer->line == '[' && *(lexer->line + 1) && (*(lexer->line + 1) == '!' || *(lexer->line + 1) == '^'))
	{
		*token = new_token(lexer, E_WILDCARD, "[!");
		lexer->bkt = 1;		
	}
	else if (*lexer->line == '[')
	{
		*token = new_token(lexer, LBKT, "[");
		lexer->bkt = 1;		
	}
	else if (*lexer->line == ']' && lexer->bkt)
	{
		*token = new_token(lexer, RBKT, "]");
		lexer->bkt = 0;
	}
	else if (*lexer->line == '{' && !lexer->bkt)
	{
		*token = new_token(lexer, LBRC, "{");
		lexer->brc = 1;
	}
	else if (*lexer->line == '}' && lexer->brc && !lexer->bkt)
	{
		*token = new_token(lexer, RBRC, "}");
		lexer->brc = 0;
	}
	else if (*lexer->line == ',' && lexer->brc && !lexer->bkt)
		*token = new_token(lexer, COM, ",");
	else if (*lexer->line == '*')
		*token = new_token(lexer, S_WILDCARD, "*");
	else if (*lexer->line == '?')
		*token = new_token(lexer, Q_WILDCARD, "?");
	else if (*lexer->line == '!')
		*token = new_token(lexer, HIST, "!");
	else if (*lexer->line == ';')
		*token = new_token(lexer, SCL, ";");
	else if (*lexer->line == '-' && last_token && (last_token->type == FRED || last_token->type == BRED))
		*token = new_token(lexer, CLOSE_FD, "-");
}

t_token		*find_token(t_lexer *lexer, t_token *last_token)
{
	t_token *token;

	token = NULL;
	if (!lexer->bs && !lexer->string_operator)
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
		else if (*lexer->line == '\\' && lexer->string_operator != '\'')
		{
			lexer->bs = 1;
			++lexer->line;
			return (NULL);
		}
		else
			find_token2(lexer, &token, last_token);
	}
	if (*lexer->line && !(token))
		token = lex_word(lexer, last_token);
	if (token && is_glob_token(token->type) && !lexer->blank && last_token && (last_token->type == WORD || last_token->type == TILD || last_token->type == NUM))
	{
		if (last_token->type == TILD)
			last_token->type = TILD_EXPR;
		else
			last_token->type = EXPR;
	}
	return (token);
}
