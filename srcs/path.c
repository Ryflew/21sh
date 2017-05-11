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
				return (check_path_end(&tmp, &dir));
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

char get_path(char **cmd, t_sh *shell, char pipe, char exec)
{
	char *content;
	char **tmp;
	int i;

	if (is_absolute(cmd, shell, pipe))
		return (0);
	if (!(content = find_env(shell->env, "PATH")))
		return (1);
	tmp = ft_strsplit(content, ':');
	content = cmd[0];
	if (ft_strrchr(content, '/'))
		content = ft_strrchr(content, '/') + 1;
	i = -1;
	while (tmp[++i])
	{
		if (check_path(content, tmp[i]))
		{
			if (exec)
				run_binary(ft_strstrjoin(tmp[i], "/", content), cmd, shell, pipe);
			ft_strdelpp(&tmp);
			return (0);
		}
	}
	ft_strdelpp(&tmp);
	return (1);
}
