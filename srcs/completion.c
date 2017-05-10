#include <sys/stat.h>
#include <sys/types.h> 
#include <dirent.h>
#include <unistd.h>
#include "21sh.h"

static char	*get_start_str(t_sh *shell)
{
	int		end;
	int		i;

	i = shell->j + 1;
	end = i;
	while (--i > -1 && shell->command[i] != ' ')
		;
	return (ft_strsub(shell->command, i + 1, end - (i + 1)));
}

static char	*check_dir_content(char *part, char *path)
{
	DIR			*dir;
	t_dirent	*ent;

	if (!(dir = opendir(path)))
		return (0);
	while ((ent = readdir(dir)))
	{
		if (!ft_strncmp(ent->d_name, part, ft_strlen(part)))
		{
			closedir(dir);
			return (ft_strdup(ent->d_name));
		}
	}
	closedir(dir);
	return (NULL);
}

static char	*find_match_binary(t_sh *shell, char *tosearch)
{
	int		i;
	char	*content;
	char	*out;
	char	**tmp;

	if (!(content = find_env(shell->env, "PATH")))
			return (NULL);
	tmp = ft_strsplit(content, ':');
	content = tosearch;
	i = -1;
	while (tmp[++i])
	{
		if ((out = check_dir_content(content, tmp[i])))
		{
			ft_strdelpp(&tmp);
			return (out);
		}
	}
	ft_strdelpp(&tmp);
	return (NULL);
}

static char	reg_or_dir(char *path)
{
	t_stat	buff;

	if (lstat(path, &buff) == -1)
		return (0);
	if (S_ISDIR(buff.st_mode) || S_ISLNK(buff.st_mode))
		return (1);
	else if (S_ISREG(buff.st_mode))
		return (2);
	return (0);
}

static void	print_missing(t_sh *shell, char *part, char *all)
{
	int		i;
	char	buff[4097];
	char	*cwd;
	char	*str;
	char	*tmp;

	i = -1;
	if (!(cwd = getcwd(buff, 4097)))
		return ;
	if (ft_strchr(part, '/'))
	{
		tmp = ft_strsub(part, 0, ft_strrchr(part, '/') - part);
		str = ft_strstrjoin(tmp, "/", all);
		free(tmp);
		if (*part != '/')
		{
			tmp = str;
			str = ft_strstrjoin(cwd, "/", tmp);
			free(tmp);
		}
		part = ft_strrchr(part, '/') + 1;
	}
	else
		str = ft_strstrjoin(cwd, "/", all);
	while (part[++i] && all[i] && part[i] == all[i])
		;
	while (all[i])
		add_char(shell->command, &(shell->j), shell, all[i++]);
	if ((i = reg_or_dir(str)) == 1)
		add_char(shell->command, &(shell->j), shell, '/');
	else if (i == 2)
		add_char(shell->command, &(shell->j), shell, ' ');
	free(str);
}

static char	*find_match_elem(char *path)
{
	char	buff[4097];
	char	*cwd;
	char	*tmp;
	char	*tmp2;

	if (*path == '/')
	{
		tmp = ft_strrchr(path, '/');
		if ((!tmp || !*(tmp + 1)) && ft_strchr(path, '/'))
			return (NULL);
		cwd = ft_strsub(path, 0, ft_strlen(path) - ft_strlen(++tmp));
	}
	else
	{
		if (!(cwd = getcwd(buff, 4097)))
			return (NULL);
		tmp = ft_strrchr(path, '/');
		if ((!tmp || !*(tmp + 1)) && ft_strchr(path, '/'))
			return (NULL);
		if (!tmp)
			tmp = path;
		tmp2 = ft_strsub(path, 0, ft_strlen(path) - ft_strlen(tmp));
		cwd = ft_strstrjoin(cwd, "/", tmp2);
		free(tmp2);
		if (*tmp == '/')
			++tmp;
	}
	if ((tmp2 = check_dir_content(tmp, cwd)))
	{
		free(cwd);
		return (tmp2);
	}
	free(cwd);
	return (NULL);
}

static void	tild_to_home(char **str, t_env *env)
{
	char	*home;
	char	*tmp;

	if ((home = find_env(env, "HOME")))
	{
		tmp = *str;
		*str = ft_strjoin(home, *str + 1);
		free(tmp);
	}
	else
	{
		free(*str);
		*str = NULL;
	}
}

void	go_completion(t_sh *shell)
{
	char	*tmp;
	char	*name;

	if (shell->j == -1)
		return ;
	shell->command[shell->j + 1] = '\0';
	if (!(tmp = get_start_str(shell)))
		return ;
	if (*tmp == '~')
		tild_to_home(&tmp, shell->env); // transforme le ~ en home
	if (!tmp)
		return ;
	if (ft_strchr(shell->command, ' ') || *tmp == '/')
		name = find_match_elem(tmp);
	else
		name = find_match_binary(shell, tmp);
	if (name)
	{
		print_missing(shell, tmp, name);
		free(name);
	}
	free(tmp);
}
