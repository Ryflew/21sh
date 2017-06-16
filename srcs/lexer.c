#include "21sh.h"

static void		skip_whitespace(t_lexer *lexer)
{
	//ft_putendl("blank");
	while (*lexer->line && ft_isblank(*lexer->line))
		++lexer->line;
}

static void		find_token2(t_lexer *lexer, t_token **token)
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

static t_token	*find_token(t_lexer *lexer)
{
	t_token *token;

	token = NULL;
	if (*lexer->line == '>' && *(lexer->line + 1) == '>')
		token = new_token(lexer, DCHEVF, ">>");
	else if (*lexer->line == '<' && *(lexer->line + 1) == '<')
		token = new_token(lexer, DCHEVB, "<<");
	else if (*lexer->line == '>' && *(lexer->line + 1) == '&')
		token = new_token(lexer, FRED, ">&");
	//else if (*lexer->line == '<' && *(lexer->line + 1) == '&')
	//	token = new_token(lexer, BRED, "<&");
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

static void		get_next_token2(t_lexer *lexer, t_token **token)
{
	if (*lexer->line && !lexer->string_operator && \
		!is_string_op(*lexer->line))
		*token = find_token(lexer);
	else if (*lexer->line && (lexer->string_operator == *lexer->line || \
		!lexer->string_operator) && *lexer->line == '\'')
		*token = new_token(lexer, QT, "'");
	else if (*lexer->line && (lexer->string_operator == *lexer->line || \
		!lexer->string_operator) && *lexer->line == '"')
		*token = new_token(lexer, DQT, "\"");
	else if (*lexer->line && (lexer->string_operator == *lexer->line || \
		!lexer->string_operator) && *lexer->line == '`')
		*token = new_token(lexer, BQT, "`");
	else
		*token = lex_str(lexer);
}

static t_token	*get_next_token(t_lexer *lexer)
{
	t_token *token;

	token = NULL;
	while (*lexer->line)
	{
		token = NULL;
		//ft_putendl(lexer->line);
		//ft_putstr("|");
		//ft_putchar(*lexer->line);
		//ft_putstr("|\n");
		if (ft_isblank(*lexer->line) && !lexer->string_operator)
		{
			skip_whitespace(lexer);
			lexer->red = 0;
		}
		else
			get_next_token2(lexer, &token);
		//ft_putendl(lexer->line);
		if (token)
			return (token);
	}
	return (token);
}

void			get_lexems(t_sh *sh)
{
	t_token *token;

	while ((token = get_next_token(sh->lexer)))
		ft_node_push_back(&(sh->lexer->lexems), token);
}
