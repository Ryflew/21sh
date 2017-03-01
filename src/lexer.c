#include "21sh.h"

void	skip_whitespace(t_lexer *lexer)
{
	while (*lexer->line && ft_isblank(*lexer->line))
		++lexer->line;
}

t_token	*new_token(e_token token_type, char *value)
{
	t_token	*token;

	if (!(token = (t_token*)malloc(sizeof(t_token))))
		exit(-1);
	token->type = token_type;
	token->value = value;
	return (token);
}

t_token	*lex_cmd(char *line)
{
	int	i;

	i = 0;
	while (line[i] && !is_operator(line[i]))
		++i;
	line[i] = 0;
	return (new_token(CMD, remove_useless_space(line)));
}

void	get_next_token(t_lexer *lexer)
{
	t_token	token;

	while (*lexer->line)
	{
		if (ft_isblank(*lexer->line))
			skip_whitespace(lexer);
		if (*lexer->line && *lexer->line == '>' && *(lexer->line + 1) == '>')
			return (new_token(DCHEVF, ">>"));
		else if (*lexer->line && *lexer->line == '<' && *(lexer->line + 1) == '<')
			return (new_token(DCHEVB, "<<"));
		else if (*lexer->line && *lexer->line == '>' && *(lexer->line + 1) == '&')
			return (new_token(FRED, ">&"));
		else if (*lexer->line && *lexer->line == '<' && *(lexer->line + 1) == '&')
			return (new_token(BRED, "<&"));
		else if (*lexer->line && *lexer->line == '>')
			return (new_token(CHEVF, ">"));
		else if (*lexer->line && *lexer->line == '<')
			return (new_token(CHEVB, "<"));
		else if (*lexer->line && *lexer->line == '&' && *(lexer->line + 1) == '&')
			return (new_token(AND, "&&"));
		else if (*lexer->line && *lexer->line == '|' && *(lexer->line + 1) == '|')
			return (new_token(OR, "||"));
		else if (*lexer->line && *lexer->line == '|')
			return (new_token(PIPE, "|"));
		else if (*lexer->line && *lexer->line == '(')
			return (new_token(LPAR, "("));
		else if (*lexer->line && *lexer->line == ')')
			return (new_token(RPAR, ")"));
		else if (*lexer->line && *lexer->line == '[')
			return (new_token(LBKT, "["));
		else if (*lexer->line && *lexer->line == ']')
			return (new_token(RBKT, "]"));
		else if (*lexer->line && *lexer->line == '{')
			return (new_token(LBRC, "{"));
		else if (*lexer->line && *lexer->line == '}')
			return (new_token(RBRC, "}"));
		else if (*lexer->line && *lexer->line == '\'')
			return (new_token(QT, "\'"));
		else if (*lexer->line && *lexer->line == '\"')
			return (new_token(DQT, "\""));
		else if (*lexer->line && *lexer->line == '\`')
			return (new_token(BQT, "\`"));
		else
			return (lex_cmd(lexer->line));
	}
}
