#include "tosh.h"

void	    replace_var(t_token *token, t_env *env)
{
	char	*to_free;
	char	*var;

	if (TYPE == VAR_WORD)
	{
		TYPE = WORD;
		to_free = VAL;
		if (!(var = find_env(env, to_free)))
			var = "";
  		VAL = ft_strdup(var);
		free(to_free);
	}
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