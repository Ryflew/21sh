#include "tosh.h"

char			is_string_op(char c)
{
	if (c == '\'' || c == '"')
		return (1);
	return (0);
}

static char	is_operator(char c, char c2)
{
	if (c == '>' || c == '<' || (c == '&' && c2 == '&') || c == '|' || \
		c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || \
		c == '}' || c == ';' || c == '*' || c == '?' || c == '`')
		return (1);
	return (0);
}

static char	is_spe_operator(char c)
{
	if (c == '`')
		return (1);
	return (0);
}

int			compute_word_size(t_lexer *lexer, e_token *type, char *st_op,
								t_token *last_token)
{
	char	bs;
	int		i;
	
	bs = 0;
	*st_op = 0;
	i = 0;
	while ((lexer->line)[i] && ((last_token && last_token->type == VAR_OP && ft_isalnum((lexer->line)[i])) || ((!last_token || last_token->type != VAR_OP) && (bs ||
		(!is_operator((lexer->line)[i], (lexer->line)[i + 1]) \
		&& !ft_isblank((lexer->line)[i]) && ((lexer->line)[i] != ',' || !lexer->brc || lexer->bkt))
		|| ((lexer->line)[i] == ']' && !lexer->bkt) || ((lexer->line)[i] == '}' && !lexer->brc)
		|| ((lexer->line)[i] == '{' && lexer->bkt) || ((lexer->line)[i] == '}' && lexer->bkt) || (*st_op && (!is_spe_operator((lexer->line)[i]) || *st_op == '\''))))))
	{
		if (*type != WORD && !ft_isdigit((lexer->line)[i]))
			*type = WORD;
		if (is_string_op((lexer->line)[i]) && !*st_op)
			*st_op = (lexer->line)[i];
		else if ((lexer->line)[i] == *st_op)
			*st_op = 0;
		if ((lexer->line)[i] == '\\' && *st_op != '\'' && !bs)
			bs = 1;
		else
			bs = 0;
		++i;
	}
	return (i);
}

t_token		*lex_word(t_lexer *lexer, t_token *last_token)
{
	int		word_size;
	t_token	*token;
	e_token	type;
	char	st_op;	

	type = NUM;
	token = NULL;
	word_size = compute_word_size(lexer, &type, &st_op, last_token);
	if (type == NUM && ((last_token && (last_token->type == FRED || last_token->type == BRED)) || ((lexer->line)[word_size] && \
	((lexer->line)[word_size] == '<' || (lexer->line)[word_size] == '>'))))
		type = FD;
	else if (isnt_here_or_bqt(lexer) && *lexer->line == '~' && (lexer->blank || !last_token || last_token->type == LBRC || last_token->type == COM) && (ft_isblank(*(lexer->line + 1)) || !*(lexer->line + 1) || *(lexer->line + 1) == '/' || *(lexer->line + 1) == '}' || *(lexer->line + 1) == ',') && !st_op)
		type = TILD;
	else if (last_token && last_token->type == VAR_OP)
		type = VAR_WORD;
	if (isnt_here_or_bqt(lexer) && last_token && is_glob_token(last_token->type) && !lexer->blank)
	{
		if (lexer->bkt)
			type = BKT_EXPR;
		else if (type == TILD)
			type = TILD_EXPR;
		else
			type = EXPR;
	}
	token = new_token(lexer, type, get_word(lexer->line, word_size));
	lexer->line += word_size - ft_strlen(VAL);
	return (token);
}
