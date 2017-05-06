#include "21sh.h"

int is_operator(int c, int c2)
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

void skip_whitespace(t_lexer *lexer)
{
	ft_putendl("blank");
	while (*lexer->line && ft_isblank(*lexer->line))
		++lexer->line;
}

t_token *new_token(t_lexer *lexer, e_token token_type, char *value)
{
	t_token *token;

	if (!(token = (t_token *)malloc(sizeof(t_token))))
		exit(-1);
	if (is_string_op(*value))
	{
		if (!lexer->string_operator)
			lexer->string_operator = *value;
		else
			lexer->string_operator = 0;
	}
	if (token_type == FRED || token_type == BRED)
		lexer->red = 1;
	else
		lexer->red = 0;
	lexer->line += ft_strlen(value);
	token->type = token_type;
	token->value = value;
	return (token);
}

t_token *lex_str(t_lexer *lexer)
{
	int i;
	t_token *token;

	i = 0;
	token = NULL;
	ft_putendl("STR");
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
	i = 0;
	while (((lexer->line)[i] && (lexer->line)[i] != ' ' && !is_operator((lexer->line)[i], (lexer->line)[i + 1])) || (string_op && (lexer->line)[i]))
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
		++i;
	}
	if (lexer->red || (type == NUM && (lexer->line)[i] && ((lexer->line)[i] == '<' || (lexer->line)[i] == '>') && (lexer->line)[i + 1] == '&'))
		type = FD;
	token = new_token(lexer, type, ft_strsub(lexer->line, 0, i));
	token->value = clear_quot(token->value);
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
	ft_putendl("WORD");
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
	token = new_token(lexer, WORD, remove_useless_space(ft_strsub(lexer->line, 0, i)));
	token->value = clear_quot(token->value);
	return (token);
}

t_token *get_next_token(t_lexer *lexer)
{
	t_token *token;

	token = NULL;
	while (*lexer->line)
	{
		token = NULL;
		ft_putendl(lexer->line);
		ft_putstr("|");
		ft_putchar(*lexer->line);
		ft_putstr("|\n");
		if (ft_isblank(*lexer->line) && !lexer->string_operator)
		{
			skip_whitespace(lexer);
			lexer->red = 0;
		}
		if (*lexer->line && !lexer->string_operator && !is_string_op(*lexer->line))
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
			else if (*lexer->line == '(')
				token = new_token(lexer, LPAR, "(");
			else if (*lexer->line == ')')
				token = new_token(lexer, RPAR, ")");
			else if (*lexer->line == '[')
				token = new_token(lexer, LBKT, "[");
			else if (*lexer->line == ']')
				token = new_token(lexer, RBKT, "]");
			else if (*lexer->line == '{')
				token = new_token(lexer, LBRC, "{");
			else if (*lexer->line == '}')
				token = new_token(lexer, RBRC, "}");
			else if (*lexer->line == ';')
				token = new_token(lexer, SCL, ";");
			else if (ft_isdigit(*lexer->line))
				token = lex_number(lexer);
			else if (*lexer->line)
				token = lex_word(lexer);
		}
		else if (*lexer->line && (lexer->string_operator == *lexer->line || !lexer->string_operator) && *lexer->line == '\'')
			token = new_token(lexer, QT, "'");
		else if (*lexer->line && (lexer->string_operator == *lexer->line || !lexer->string_operator) && *lexer->line == '"')
			token = new_token(lexer, DQT, "\"");
		else if (*lexer->line && (lexer->string_operator == *lexer->line || !lexer->string_operator) && *lexer->line == '`')
			token = new_token(lexer, BQT, "`");
		else
			token = lex_str(lexer);
		ft_putendl(lexer->line);
		if (token)
			return (token);
	}
	return (token);
}
