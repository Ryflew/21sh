#include "tosh.h"

static char	*add_var_in_cmdline(t_env *env, int *i, char *cmd)
{
	int		j;
	char	*var;
	char	*to_free;
	char	*left;
	
	left = ft_strsub(cmd, 0, *i);
	j = *i;
	while (cmd[++j] && ft_isalnum(cmd[j]))
		;
	var = ft_strsub(cmd, *i + 1, j - *i - 1);
	to_free = var;
	var = find_env(env, var);
	if (to_free)
		free(to_free);
	to_free = cmd;
	cmd = (var) ? ft_strstrjoin(left, var, cmd + j) : ft_strjoin(left, cmd + j);
	if (to_free)
		free(to_free);
	if (left)
		free(left);
	if (var)
		*i += ft_strlen(var);
	return (cmd);
}

char	    *replace_var(t_sh *sh, char *cmd)
{
    int		i;
    char    st_op;
    char    bs;

	i = -1;
	bs = 0;
	st_op = 0;
	while (cmd[++i])
	{
		if (cmd[i] == '\\' && !st_op)
			bs = 1;
		else if (bs)
			bs = 0;
		else if (cmd[i] == '\'' && !bs)
		{
			if (!st_op)
                st_op = '\'';
			else
                st_op = 0;
		}
		else if (cmd[i] == '$' && !bs
				&& !st_op && ft_isalnum(cmd[i + 1]))
			cmd = add_var_in_cmdline(sh->env, &i, cmd);
	}
	return (cmd);
}