#include "21sh.h"

int			is_operator(int c, int c2)
{
	if (c == '>' || c == '<' || (c == '&' && c2 == '&') || c == '|' || c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}' || c == '"' || c == '\'' || c == '`')
					return (1);
	return (0);
}

int			is_string_op(int c)
{
	if (c == QT || c == DQT || c == BQT)
					return (1);
	return (0);
}

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
	if (is_string_op(token_type))
		lexer->string_operator = *value;
	else
		lexer->string_operator = 0;
	token->type = token_type;
	token->value = value;
	return (token);
}

t_token	*lex_str(char *line, char string_operator, t_lexer *lexer)
{
	int	i;

	i = 0;
	while (line && *line != string_operator)
	{
		++i;
		++line;
	}
	return (new_token(lexer, TEXT, remove_useless_space(ft_strsub(line -= i - 1, 0, i))));
}

t_token	*lex_number(char *line, t_lexer *lexer)
{
	int		i;
	e_token	type;

	type = NUM;
	i = 0;
	while (line[i] && line[i] != ' ' && !is_operator(line[i], line[i + 1]))
	{
		if (!ft_isdigit(line[i]))
			type = WORD;
		++i;
	}
	return (new_token(lexer, type, ft_strsub(line += i - 1, 0, i)));
}

t_token	*lex_word(char *line, t_lexer *lexer)
{
	int	i;

	i = 0;
	ft_putendl(line);
	while (line[i] && !is_operator(line[i], line[i + 1]))
		++i;
	ft_putnbr(i);
	ft_putendl(ft_strsub(line, 0, i));
	return (new_token(lexer, WORD, ft_strsub(line += i - 1, 0, i)));
}

t_token	*get_next_token(t_lexer *lexer)
{
	t_token	*token;

	token = NULL;
	while (*lexer->line)
	{
		token = NULL;
		if (ft_isblank(*lexer->line))
			skip_whitespace(lexer);
		if (!lexer->string_operator)
		{
			if (*lexer->line && *lexer->line == '>' && *(lexer->line + 1) == '>')
				token = new_token(lexer, DCHEVF, ">>");
			else if (*lexer->line && *lexer->line == '<' && *(lexer->line + 1) == '<')
				token = new_token(lexer, DCHEVB, "<<");
			else if (*lexer->line && *lexer->line == '>' && *(lexer->line + 1) == '&')
				token = new_token(lexer, FRED, ">&");
			else if (*lexer->line && *lexer->line == '<' && *(lexer->line + 1) == '&')
				token = new_token(lexer, BRED, "<&");
			else if (*lexer->line && *lexer->line == '>')
				token = new_token(lexer, CHEVF, ">");
			else if (*lexer->line && *lexer->line == '<')
				token = new_token(lexer, CHEVB, "<");
			else if (*lexer->line && *lexer->line == '&' && *(lexer->line + 1) == '&')
				token = new_token(lexer, AND, "&&");
			else if (*lexer->line && *lexer->line == '|' && *(lexer->line + 1) == '|')
				token = new_token(lexer, OR, "||");
			else if (*lexer->line && *lexer->line == '|')
				token = new_token(lexer, PIPE, "|");
			else if (*lexer->line && *lexer->line == '(')
				token = new_token(lexer, LPAR, "(");
			else if (*lexer->line && *lexer->line == ')')
				token = new_token(lexer, RPAR, ")");
			else if (*lexer->line && *lexer->line == '[')
				token = new_token(lexer, LBKT, "[");
			else if (*lexer->line && *lexer->line == ']')
				token = new_token(lexer, RBKT, "]");
			else if (*lexer->line && *lexer->line == '{')
				token = new_token(lexer, LBRC, "{");
			else if (*lexer->line && *lexer->line == '}')
				token = new_token(lexer, RBRC, "}");
			else if (*lexer->line && *lexer->line == '\'')
				token = new_token(lexer, QT, "'");
			else if (*lexer->line && *lexer->line == '"')
				token = new_token(lexer, DQT, "\"");
			else if (*lexer->line && *lexer->line == '`')
				token = new_token(lexer, BQT, "`");
			else if (*lexer->line && ft_isdigit(*lexer->line))
				token = lex_number(lexer->line, lexer);
			else if (*lexer->line)
				token = lex_word(lexer->line, lexer);
		}
		else
			token = lex_str(lexer->line, lexer->string_operator, lexer);
		++lexer->line;
		if (token)
			return (token);
	}
	return (token);
}
