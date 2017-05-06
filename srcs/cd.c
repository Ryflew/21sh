#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "21sh.h"

static void	change_path(char *path, t_env *env, t_sh *shell)
{
	char	**av;
	char	buff[4097];
	char	*tmp;

	tmp = getcwd(buff, 4097);
	if (cd_path_validity(tmp))
	{
		if (!(av = (char**)malloc(sizeof(char*) * 4)))
			exit(EXIT_FAILURE);
		av[0] = ft_strdup("setenv");
		av[1] = ft_strdup("OLDPWD");
		av[2] = ft_strdup(tmp);
		av[3] = NULL;
		set_env(av, &env);
		ft_strdelpp(&av);
	}
	tmp = ft_strdup(path);
	change_prompt(tmp, env);
	free(tmp);
	if (!(av = (char**)malloc(sizeof(char*) * 4)))
		exit(EXIT_FAILURE);
	av[0] = ft_strdup("setenv");
	av[1] = ft_strdup("PWD");
	av[2] = ft_strdup(getcwd(buff, 4097));
	av[3] = NULL;
	free(shell->prompt);
	shell->prompt = get_with_tilde(getcwd(buff, 4097), env);
	set_env(av, &env);
	ft_strdelpp(&av);
}

static void	print_cd_error(char *tmp, char *path)
{
	t_stat	file;

	if (!lstat(tmp, &file) && S_ISREG(file.st_mode))
		errexit(path, "Not a directory.");
	else if (access(tmp, F_OK) == -1)
		errexit("cd", "No such file or directory.");
	else
		errexit(tmp, "Permission denied.");
}

static void	cd_current_dir(char *path, t_env *env, t_sh *shell)
{
	char	*cwd;
	char	*tmp;
	char	buff[4097];

	if (*path == '/' || (cwd = getcwd(buff, 4097))) // edit pour pouvoir faire .. quand on del un dir
	{
		if (cd_path_validity(path))
			change_path(path, env, shell);
		else
		{
			if (cwd && *path != '/')
			{
				tmp = ft_strstrjoin(cwd, "/", path);
				print_cd_error(tmp, path);
				free(tmp);
			}
			else
				print_cd_error(path, path);
		}
	}
	else
		print_cd_error(path, path);
}

static void	cd_tilde(char *str, t_env *env, t_sh *shell)
{
	char	*tmp;
	char	*tmp2;

	if (!(tmp = find_env(env, "HOME")))
	{
		errexit("cd", "HOME not set.");
		return ;
	}
	if (!str)
		tmp2 = ft_strdup(tmp);
	else
		tmp2 = ft_strjoin(tmp, str + 1);
	cd_current_dir(tmp2, env, shell);
	free(tmp2);
}

void	cd(char **av, t_env *env, t_sh *shell)
{
	char	*tmp;

	if (!*++av || (*av[0] == '~'))
		cd_tilde(*av, env, shell);
	else if (*(av + 1))
		errexit("cd", "Too many arguments.");
	else if (!ft_strcmp(*av, "-") && find_env(env, "OLDPWD"))
	{
		tmp = ft_strdup(find_env(env, "OLDPWD"));
		change_path(tmp, env, shell);
		free(tmp);
	}
	else if (*av[0] == '/')
	{
		if (cd_path_validity(*av))
			change_path(*av, env, shell);
		else
		{
			if (!lstat(*av, NULL))
				errexit("cd", "Not a directory.");
			else
				errexit("cd", "No such file or directory.");
		}
	}
	else
		cd_current_dir(*av, env, shell);
}