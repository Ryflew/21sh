#include "21sh.h"
#include <sys/stat.h>
#include <dirent.h>

static char check_path_end(char **tmp, DIR **dir)
{
	free(*tmp);
	closedir(*dir);
	return (1);
}

static char check_path(char *command, char *path)
{
	DIR *dir;
	t_dirent *ent;
	t_stat file;
	char *tmp;

	if (!(dir = opendir(path)))
		return (0);
	while ((ent = readdir(dir)))
	{
		if (!ft_strcmp(ent->d_name, command))
		{
			tmp = ft_strstrjoin(path, "/", ent->d_name);
			if (!stat(tmp, &file) && S_ISREG(file.st_mode) && is_binary(tmp) &&
				!access(tmp, R_OK | X_OK))
				{
					if (!is_in_hashtab(tmp, get_shell()->hash))
						add_hash_line(ent->d_name, tmp, get_shell());
					return (check_path_end(&tmp, &dir));
				}
			else if (access(tmp, R_OK | X_OK) == -1)
			{
				errexit(tmp, "Permission denied.");
				return (check_path_end(&tmp, &dir));
			}
			free(tmp);
		}
	}
	closedir(dir);
	return (0);
}

char get_path(t_tree *node, t_env *env, t_sh *shell, char exec)
{
	char *content;
	char **tmp;
	int i;

	if ((content = find_env(shell->hash, node->cmds[0])))
	{
		if (exec)
			run_binary(content, node, env, shell);
		return (0);
	}
	if (is_absolute(node, env, shell) && exec)
		return (1);
	if (!(content = find_env(env, "PATH")))
		return (1);
	tmp = ft_strsplit(content, ':');
	content = node->cmds[0];
	if (ft_strrchr(content, '/'))
		content = ft_strrchr(content, '/') + 1;
	i = -1;
	while (tmp[++i])
	{
		if (check_path(content, tmp[i]))
		{
			if (exec)
				run_binary(ft_strstrjoin(tmp[i], "/", content), node, env, shell);
			ft_strdelpp(&tmp);
			return (0);
		}
	}
	ft_strdelpp(&tmp);
	return (1);
}

char	is_absolute(t_tree *node, t_env *env, t_sh *shell)
{
	t_stat		file;

	if (**node->cmds && **node->cmds == '/')
	{
		if (!lstat(*node->cmds, &file) && S_ISREG(file.st_mode) && is_binary(*node->cmds) &&
			!access(*node->cmds, R_OK | X_OK))
		{
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
	}
	else if (**node->cmds && **node->cmds == '.' && *(*node->cmds + 1) && *(*node->cmds + 1) == '/')
		return (current_binary(node, env, shell));
	return (0);
}