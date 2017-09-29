#include "tosh.h"

static char	*replace_var(t_sh *sh, char *command)
{
	int		i;
	int		j;
	char	*left;
	char	*var;
	char	*to_free;

	i = -1;
	sh->lexer->bs = 0;
	sh->lexer->string_operator = 0;
	while (command[++i])
	{
		if (command[i] == '\\' && !sh->lexer->string_operator)
			sh->lexer->bs = 1;
		else if (sh->lexer->bs)
			sh->lexer->bs = 0;
		else if (command[i] == '\'' && !sh->lexer->bs)
		{
			if (!sh->lexer->string_operator)
				sh->lexer->string_operator = '\'';
			else
				sh->lexer->string_operator = 0;
		}
		else if (command[i] == '$' && !sh->lexer->bs && !sh->lexer->string_operator && ft_isalnum(command[i + 1]))
		{
			left = ft_strsub(command, 0, i);
			j = i;
			while (command[++j] && ft_isalnum(command[j]))
				;
			var = ft_strsub(command, i + 1, j - i - 1);
			to_free = var;
			var = find_env(sh->env, var);
			if (to_free)
				free(to_free);
			to_free = command;
			if (var)
				command = ft_strstrjoin(left, var, command + j);
			else
				command = ft_strjoin(left, command + j);
			if (to_free)
				free(to_free);
			if (left)
				free(left);
			if (var)
				i += ft_strlen(var);
		}
	}
	return (command);
}

static void		get_next_token2(t_lexer *lexer, t_token **token, t_token *last_token)
{
	if (*lexer->line && *lexer->line == '\'' && !lexer->bs)
		manage_string_op(lexer);
	else if (*lexer->line && (lexer->string_operator == *lexer->line || \
		!lexer->string_operator) && *lexer->line == '"' && !lexer->bs)
		manage_string_op(lexer);
	else if (*lexer->line)
		*token = find_token(lexer, last_token);
}

static t_token	*get_next_token(t_lexer *lexer, t_token *last_token)
{
	t_token *token;

	token = NULL;
	while (*lexer->line)
	{
		token = NULL;
		if (ft_isblank(*lexer->line) && !lexer->string_operator && !lexer->bs)
		{
			while (*lexer->line && ft_isblank(*lexer->line))
				++lexer->line;
			lexer->blank = 1;
			if (last_token && (is_glob_token(last_token->type) || last_token->type == EXPR || last_token->type == TILD_EXPR || last_token->type == BKT_EXPR || last_token->type == COM))
				return (new_token(lexer, NONE, ""));
		}
		else
		{
			get_next_token2(lexer, &token, last_token);
			lexer->blank = 0;
		}
		if (token)
			return (token);
	}
	if (last_token && (is_glob_token(last_token->type) || last_token->type == EXPR || last_token->type == TILD_EXPR || last_token->type == BKT_EXPR || last_token->type == COM))
		return (new_token(lexer, NONE, ""));
	return (token);
}

void			init_lexer(t_lexer *lexer, char *command)
{
	lexer->lexems = NULL;
	lexer->bs = 0;
	lexer->line = command;
	lexer->brc = 0;
	lexer->bkt = 0;
	lexer->bqt = 0;
	lexer->blank = 0;
	lexer->string_operator = 0;
}

void			replace_tild(t_list *lexems, t_env *env)
{
	t_token *token;
	char	*to_free;	

	while (lexems)
	{
		token = (t_token*)lexems->data;
		if (token->type == TILD)
		{
			token->type = WORD;
			to_free = token->value;
			token->value = ft_strjoin(find_env(env, "HOME"), token->value + 1);
			free(to_free);
		}
		else if (token->type == TILD_EXPR)
		{
			token->type = EXPR;
			to_free = token->value;
			token->value = ft_strjoin(find_env(env, "HOME"), token->value + 1);
			free(to_free);
		}
		lexems = lexems->next;
	}
}

void			get_lexems(t_sh *sh)
{
	t_token *token;

	sh->total_command = replace_var(sh, sh->total_command);
	
	init_lexer(sh->lexer, sh->total_command);
	token = NULL;
	while ((token = get_next_token(sh->lexer, token)))
		ft_node_push_back(&(sh->lexer->lexems), token);
	replace_tild(sh->lexer->lexems, sh->env);
	/*t_list *tmp = sh->lexer->lexems;
	while (tmp)
	{
		ft_putnbr(((t_token*)(tmp->data))->type);
		ft_putendl(" <-- type");
		ft_putendl(((t_token*)(tmp->data))->value);
		tmp = tmp->next;
	}*/
}
