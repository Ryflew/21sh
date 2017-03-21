#include "21sh.h"

void	skip_whitespace(t_lexer *lexer)
{
	while (*lexer->line && ft_isblank(*lexer->line))
		++lexer->line;
}

t_token	*new_token(t_lexer *lexer, e_token token_type, char *value)
{
	t_token	*token;

	if (!(token = (t_token*)malloc(sizeof(t_token))))
		exit(-1);
	if (*value == '"' || *value == "\"" || value == "`")
		lexer->string_operator = *value;
	else
		lexer->string_operator = 0;
	token->type = token_type;
	token->value = value;
	return (token);
}

t_token	*lex_cmd(char *line)
{
	int	i;

	i = 0;
	while (line[i] && (string_operator && (line[i] != string_operator) || !is_operator(line[i])))
		++i;
	line[i] = 0;
	return (new_token(lexer, CMD, remove_useless_space(line)));
}

void	get_next_token(t_lexer *lexer)
{
	t_token	token;

	while (*lexer->line)
	{
		if (ft_isblank(*lexer->line))
			skip_whitespace(lexer);
		if (!lexer->string_operator)
		{
			if (*lexer->line && *lexer->line == '>' && *(lexer->line + 1) == '>')
				return (new_token(lexer, DCHEVF, ">>"));
			else if (*lexer->line && *lexer->line == '<' && *(lexer->line + 1) == '<')
				return (new_token(lexer, DCHEVB, "<<"));
			else if (*lexer->line && *lexer->line == '>' && *(lexer->line + 1) == '&')
				return (new_token(lexer, FRED, ">&"));
			else if (*lexer->line && *lexer->line == '<' && *(lexer->line + 1) == '&')
				return (new_token(lexer, BRED, "<&"));
			else if (*lexer->line && *lexer->line == '>')
				return (new_token(lexer, CHEVF, ">"));
			else if (*lexer->line && *lexer->line == '<')
				return (new_token(lexer, CHEVB, "<"));
			else if (*lexer->line && *lexer->line == '&' && *(lexer->line + 1) == '&')
				return (new_token(lexer, AND, "&&"));
			else if (*lexer->line && *lexer->line == '|' && *(lexer->line + 1) == '|')
				return (new_token(lexer, OR, "||"));
			else if (*lexer->line && *lexer->line == '|')
				return (new_token(lexer, PIPE, "|"));
			else if (*lexer->line && *lexer->line == '(')
				return (new_token(lexer, LPAR, "("));
			else if (*lexer->line && *lexer->line == ')')
				return (new_token(lexer, RPAR, ")"));
			else if (*lexer->line && *lexer->line == '[')
				return (new_token(lexer, LBKT, "["));
			else if (*lexer->line && *lexer->line == ']')
				return (new_token(lexer, RBKT, "]"));
			else if (*lexer->line && *lexer->line == '{')
				return (new_token(lexer, LBRC, "{"));
			else if (*lexer->line && *lexer->line == '}')
				return (new_token(lexer, RBRC, "}"));
			else if (*lexer->line && *lexer->line == ''')
				return (new_token(lexer, QT, "'"));
			else if (*lexer->line && *lexer->line == '"')
				return (new_token(lexer, DQT, "\""));
			else if (*lexer->line && *lexer->line == '`')
				return (new_token(lexer, BQT, "`"));
		}
		return (lex_cmd(lexer->line), lexer->string_operator);
	}
}
