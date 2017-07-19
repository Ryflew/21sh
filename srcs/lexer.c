#include "21sh.h"

int			is_string_op(int c)
{
	if (c == '\'' || c == '"' || c == '`')
		return (1);
	return (0);
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

static void		manage_quote2(t_lexer *lexer, char *tmp)
{
	int		i;
	int		j;
	int		k;
	char	buff[ft_strlen(lexer->line) + 1];

	i = 1;
	j = -1;
	k = -1;
	ft_bzero(buff, ft_strlen(lexer->line) + 1);
	buff[++k] = tmp[++j];
	while (tmp[++j] && (tmp[j] != ' ' || i == 1))
	{
		if (tmp[j] != tmp[0])
			buff[++k] = tmp[j];
		else
			i = i == 0 ? 1 : 0;
	}
	buff[++k] = tmp[0];
	while (tmp[j])
		buff[++k] = tmp[j++];
	buff[++k] = '\0';
	lexer->line = ft_strdup(buff);
	free(tmp);
}

void		manage_string_op(t_lexer *lexer)
{
	if (!lexer->string_operator)
		lexer->string_operator = *lexer->line;
	else if (*lexer->line == lexer->string_operator)
		lexer->string_operator = 0;
	if (*lexer->line != '`')
		++lexer->line;
}

static void		manage_quote(t_lexer *lexer)
{
	int		i;
	int		j;
	int		k;
	char	buff[ft_strlen(lexer->line) + 1];

	if (lexer->string_operator || (!(lexer->line + 1) && (*lexer->line == '\'' || *lexer->line == '\"' || *lexer->line == '`')))
		return ;
	i = -1;
	while (lexer->line[++i] && lexer->line[i] != '\'' && lexer->line[i] != '"' \
		&& lexer->line[i] != '`' && lexer->line[i] != ' ')
		;
	if (!lexer->line[i] || lexer->line[i] == ' ')
		return ;
	k = -1;
	buff[++k] = lexer->line[i];
	j = -1;
	while (lexer->line[++j])
	{
		if (j == i)
			continue ;
		buff[++k] = lexer->line[j];
	}
	buff[++k] = '\0';
	manage_quote2(lexer, ft_strdup(buff));
}

static void		get_next_token2(t_lexer *lexer, t_token **token)
{
	manage_quote(lexer);
	if (*lexer->line && (lexer->string_operator == *lexer->line || \
		!lexer->string_operator) && *lexer->line == '\'')
		manage_string_op(lexer);
	else if (*lexer->line && (lexer->string_operator == *lexer->line || \
		!lexer->string_operator) && *lexer->line == '"')
		manage_string_op(lexer);
	else if (*lexer->line && (lexer->string_operator == *lexer->line || \
		!lexer->string_operator) && *lexer->line == '`')
	{
		manage_string_op(lexer);
		*token = new_token(lexer, BQT, "`");
	}
	else if (*lexer->line)
		*token = find_token(lexer);
}

static t_token	*get_next_token(t_lexer *lexer)
{
	t_token *token;

	token = NULL;
	while (*lexer->line)
	{
		token = NULL;
		if (ft_isblank(*lexer->line) && !lexer->string_operator)
		{
			while (*lexer->line && ft_isblank(*lexer->line))
				++lexer->line;
			lexer->red = 0;
		}
		else
			get_next_token2(lexer, &token);
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
