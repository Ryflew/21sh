#include "tosh.h"

static void	concat_var(char her, t_list **lexems, t_token *token)
{
	t_token	*last_token;
	char	*to_free;
	
	last_token = (t_token*)(*lexems)->prev->data;
	if (last_token->type != WORD && last_token->type != NUM)
		return ;
	to_free = last_token->value;
	last_token->value = ft_strjoin(to_free, VAL);
	free(to_free);
	if (her)
		ft_pop_node(lexems, (void*)&clear_lexems);
	else
		ft_pop_node(lexems, NULL);
}

static void	replace_var(e_token last_type, t_list **lexems, t_sh *sh)
{
	char	*to_free;
	t_token	*token;
	char	*var;

	token = (t_token*)(*lexems)->data;
	if (TYPE == VAR_WORD)
	{
		TYPE = WORD;
		to_free = VAL;
		if (!(var = find_env(sh->env, to_free)))
			if (!(var = find_env(sh->export, to_free)))
				if (!(var = find_env(sh->shell_var, to_free)))
					var = "";
  		VAL = ft_strdup(var);
		free(to_free);
	}
	if ((*lexems)->prev && last_type == VAR_OP_C)
		concat_var(sh->lexer->her, lexems, token);
	if ((*lexems)->prev && ((t_token*)(*lexems)->prev->data)->type == EQUAL)
		TYPE = VAR_WORD;
}

void		manage_var_op(t_sh *sh, t_list **tmp, t_list **cmd_tokens,
					t_token *token)
{
	e_token last_type;
	
	last_type = TYPE;
	if (*tmp == *cmd_tokens)
		*cmd_tokens = (*cmd_tokens)->next;
	if (!sh->lexer->her)
		ft_pop_node(tmp, NULL);
	else
	{
		if (*tmp == *cmd_tokens)
			*cmd_tokens = (*tmp)->next;
		ft_pop_node(tmp, (void*)&clear_lexems);
	}
	if (*tmp)
		replace_var(last_type, tmp, sh);
}

void		replace_tild(t_token *token, t_env *env)
{
	char	*to_free;	

	if (TYPE == TILD)
	{
		TYPE = WORD;
		to_free = VAL;
		VAL = ft_strjoin(find_env(env, "HOME"), VAL + 1);
		free(to_free);
	}
	else if (TYPE == TILD_EXPR)
	{
		TYPE = EXPR;
		to_free = VAL;
		VAL = ft_strjoin(find_env(env, "HOME"), VAL + 1);
		free(to_free);
	}
}