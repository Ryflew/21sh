#include "tosh.h"

static void		init_lexer(t_lexer *lexer, char *command)
{
	lexer->lexems = NULL;
	lexer->line = command;
	lexer->brc = 0;
	lexer->bkt = 0;
	lexer->bqt = 0;
	lexer->blank = 0;
}

static t_token	*get_next_token(t_lexer *lexer, t_token *last_token)
{
	t_token *token;

	token = NULL;
	while (*lexer->line)
	{
		token = NULL;
		if (ft_isblank(*lexer->line))
		{
			while (*lexer->line && ft_isblank(*lexer->line))
				++lexer->line;
			lexer->blank = 1;
			if (last_token && (is_glob_token(last_token->type) || last_token->type == EXPR || last_token->type == TILD_EXPR || last_token->type == BKT_EXPR || last_token->type == COM))
				return (new_token(lexer, END_EXPR, ""));
		}
		else
		{
			token = identify_token(lexer, last_token);
			lexer->blank = 0;
		}
		if (token)
			return (token);
	}
	if (last_token && (is_glob_token(last_token->type) || last_token->type == EXPR || last_token->type == TILD_EXPR || last_token->type == BKT_EXPR || last_token->type == COM))
		return (new_token(lexer, END_EXPR, ""));
	return (token);
}

void			get_lexems(t_sh *sh)
{
	t_token *token;

	//sh->total_command = replace_var(sh, sh->total_command);
	init_lexer(sh->lexer, sh->total_command);
	token = NULL;
	while ((token = get_next_token(sh->lexer, token)))
		ft_node_push_back(&(sh->lexer->lexems), token);
//	replace_tild(sh->lexer->lexems, sh->env);
	/*t_list *tmp = sh->lexer->lexems;
	while (tmp)
	{
		ft_putnbr(((t_token*)(tmp->data))->type);
		ft_putendl(" <-- type");
		ft_putendl(((t_token*)(tmp->data))->value);
		tmp = tmp->next;
	}*/
}
