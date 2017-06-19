#include "21sh.h"
#include <sys/stat.h>
#include <dirent.h>

static char	check_in_hashtab(t_tree *node, t_env *env, t_sh *shell, char exec)
{
	char	*content;

	if ((content = find_env(shell->hash, node->cmds[0])))
	{
		if (exec)
			run_binary(ft_strdup(content), node, env, shell);
		return (1);
	}
	return (0);
}

static void	fuck_norme(char ***tmp, char **content, t_tree *node)
{
	*tmp = ft_strsplit(*content, ':');
	*content = node->cmds[0];
	if (ft_strrchr(*content, '/'))
		*content = ft_strrchr(*content, '/') + 1;
}

char		get_path(t_tree *node, t_env *env, t_sh *shell, char exec)
{
	char	*content;
	char	**tmp;
	int		i;

	if (node->cmds && check_in_hashtab(node, env, shell, exec))
		return (0);
	if (is_absolute(node, env, shell, exec))
		return (2);
	if (!(content = find_env(env, "PATH")))
		return (1);
	fuck_norme(&tmp, &content, node);
	i = -1;
	while (tmp[++i])
	{
		if (check_path(content, tmp[i]))
		{
			if (exec)
				run_binary(ft_strstrjoin(tmp[i], "/", content), \
					node, env, shell);
			ft_strdelpp(&tmp);
			return (0);
		}
	}
	ft_strdelpp(&tmp);
	return (1);
}

static char	is_absolute2(t_tree *node, t_env *env, t_sh *shell, char exec)
{
	t_stat	file;

	if (!lstat(*node->cmds, &file) && S_ISREG(file.st_mode) && \
		is_binary(*node->cmds) && !access(*node->cmds, R_OK | X_OK))
	{
		if (exec)
			run_binary(ft_strdup(*node->cmds), node, env, shell);
		return (1);
	}
	else if (access(*node->cmds, F_OK) == -1)
	{
		errexit(*node->cmds, "No such file or directory.");
		return (1);
	}
	else if (access(*node->cmds, R_OK | X_OK) == -1)
	{
		errexit(*node->cmds, "Permission denied.");
		return (1);
	}
	return (0);
}

char		is_absolute(t_tree *node, t_env *env, t_sh *shell, char exec)
{
	if (node->cmds && **node->cmds && **node->cmds == '/')
		return (is_absolute2(node, env, shell, exec));
	else if (node->cmds && **node->cmds && **node->cmds == '.' && \
		*(*node->cmds + 1) && *(*node->cmds + 1) == '/')
	{
		if (exec)
			return (current_binary(node, env, shell));
	}
	return (0);
}
