#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include "21sh.h"

static char	check_path_end(char **tmp, DIR **dir)
{
	free(*tmp);
	closedir(*dir);
	return (1);
}

static char	check_path2(t_dirent *ent, char *tmp)
{
	t_stat		file;

	if (!stat(tmp, &file) && S_ISREG(file.st_mode) && is_binary(tmp) &&
		!access(tmp, R_OK | X_OK))
	{
		if (!is_in_hashtab(tmp, get_shell()->hash))
			add_hash_line(ent->d_name, tmp, get_shell());
		return (1);
	}
	else if (access(tmp, R_OK | X_OK) == -1)
	{
		errexit(tmp, "Permission denied.");
		return (1);
	}
	return (0);
}

char		check_path(char *command, char *path)
{
	DIR			*dir;
	t_dirent	*ent;
	char		*tmp;

	if (!(dir = opendir(path)))
		return (0);
	while ((ent = readdir(dir)))
	{
		if (!ft_strcmp(ent->d_name, command))
		{
			tmp = ft_strstrjoin(path, "/", ent->d_name);
			if (check_path2(ent, tmp))
				return (check_path_end(&tmp, &dir));
			free(tmp);
		}
	}
	closedir(dir);
	return (0);
}
