#include "tosh.h"

t_env		*cpy_env(t_env *env)
{
	t_env	*elem;
	t_env	*first;

	first = NULL;
	if (env)
	{
		elem = new_env(env->all);
		first = elem;
		env = env->next;
	}
	while (env)
	{
		elem->next = new_env(env->all);
		elem = elem->next;
		env = env->next;
	}
	return (first);
}

static void	check_if_env_var2(t_tree *tree, t_env *cpy, char *ptr, int i)
{
	char	*tmp;

	while (cpy)
	{
		if (!ft_strcmp(cpy->var_name, ptr + 1))
		{
			tmp = ft_strsub(tree->cmds[i], 0, ptr - tree->cmds[i]);
			free(tree->cmds[i]);
			tree->cmds[i] = ft_strjoin(tmp, cpy->var_content);
			free(tmp);
			return ;
		}
		NEXT(cpy);
	}
}

void		check_if_env_var(t_tree *tree)
{
	int		i;
	t_env	*cpy;
	char	*ptr;

	if (tree->left)
		return (check_if_env_var(tree->left));
	if (tree->right)
		return (check_if_env_var(tree->right));
	i = -1;
	cpy = get_shell()->env;
	while (tree->cmds && tree->cmds[++i])
	{
		if (!(ptr = ft_strchr(tree->cmds[i], '$')) && \
			!find_env(cpy, tree->cmds[i]))
			continue ;
			check_if_env_var2(tree, cpy, ptr, i);
	}
}
