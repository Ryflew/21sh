#include "tosh.h"

static void	is_here_op(t_lexer *lexer, t_token **token, t_token *last_token)
{
	if (*lexer->line == '`')
	{
		if (!lexer->bqt)
			*token = (lexer->blank || !last_token) ? new_token(lexer, BQT, "`") \
			: new_token(lexer, BQT_C, "`");
		else
			*token = new_token(lexer, EBQT, "`");
		lexer->bqt = !lexer->bqt;
	}
	else if (*lexer->line == '$' && ft_isalnum(*(lexer->line + 1)))
	{
		if (!lexer->blank && last_token)
			*token = new_token(lexer, VAR_OP_C, "$");
		else
			*token = new_token(lexer, VAR_OP, "$");
	}
}

static void	is_other_op(t_lexer *lexer, t_token **token, t_token *last_token)
{
	if (*lexer->line == ',' && lexer->brc && !lexer->bkt)
		*token = new_token(lexer, COM, ",");
	else if (*lexer->line == '*')
		*token = new_token(lexer, S_WILDCARD, "*");
	else if (*lexer->line == '?')
		*token = new_token(lexer, Q_WILDCARD, "?");
	else if (*lexer->line == '(')
		*token = new_token(lexer, LPAR, "(");
	else if (*lexer->line == ')')
		*token = new_token(lexer, RPAR, ")");
	else if (*lexer->line == '!')
		*token = new_token(lexer, HIST, "!");
	else if (*lexer->line == ';')
		*token = new_token(lexer, SCL, ";");
	else if (*lexer->line == '-' && last_token
			&& (last_token->type == FRED || last_token->type == BRED))
		*token = new_token(lexer, CLOSE_FD, "-");
	else if (*lexer->line == '=' && !lexer->blank && last_token &&
			(last_token->type == ASCII_WORD || last_token->type == NUM))
	{
		*token = new_token(lexer, EQUAL, "=");
		last_token->type = VAR_WORD;
	}
}

static void	is_limit_glob_op(t_lexer *lexer, t_token **token)
{
	if (*lexer->line == '[')
	{ 
		if (*(lexer->line + 1) && (*(lexer->line + 1) == '!'
			|| *(lexer->line + 1) == '^'))
			*token = new_token(lexer, E_WILDCARD, "[!");
		else
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
}


static t_token	*is_reg_op(t_lexer *lexer)
{
	t_token *token;

	token = NULL;
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
	return (token);
}

t_token			*identify_token(t_lexer *lexer, t_token *l_tk)
{
	t_token *token;

	token = NULL;
	if (isnt_here_or_bqt(lexer))
		token = is_reg_op(lexer);
	if (!token && isnt_here_or_bqt(lexer))
		is_limit_glob_op(lexer, &token);
	if (!token && isnt_here_or_bqt(lexer))
		is_other_op(lexer, &token, l_tk);
	if (!token)
		is_here_op(lexer, &token, l_tk);
	if (*lexer->line && !(token))
		token = lex_word(lexer, l_tk);
	if (isnt_here_or_bqt(lexer) && token && is_glob_token(TYPE) &&
	!lexer->blank && l_tk && (l_tk->type == WORD ||
		l_tk->type == TILD || l_tk->type == NUM || l_tk->type == ASCII_WORD))
	{
		if (l_tk->type == TILD)
			l_tk->type = TILD_EXPR;
		else
			l_tk->type = EXPR;
	}
	return (token);
}
