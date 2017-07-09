#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "21sh.h"

static void	change_path(char *path, t_env *env, t_sh *shell, t_cd *opt)
{
	char	**av;
	char	buff[4097];
	char	*tmp;
	char	*new_prompt;

	tmp = getcwd(buff, 4097);
	if (cd_path_validity(tmp))
		init_setenv(&av, &env, tmp);
	new_prompt = NULL;
	tmp = ft_strdup(path);
	change_prompt(tmp, env, &new_prompt, opt);
	free(tmp);
	if (!(av = (char**)malloc(sizeof(char*) * 4)))
		exit(EXIT_FAILURE);
	av[0] = ft_strdup("setenv");
	av[1] = ft_strdup("PWD");
	av[2] = ft_strdup(new_prompt);
	av[3] = NULL;
	free(shell->prompt);
	shell->prompt = get_with_tilde(new_prompt, env);
	free(new_prompt);
	set_env(av, &env);
	ft_strdelpp(&av);
}

void		cd_current_dir(char *path, t_env *env, t_sh *shell, t_cd *opt)
{
	char	*cwd;
	char	*tmp;
	char	buff[4097];

	if (*path == '/' || (cwd = getcwd(buff, 4097)))
	{
		if (cd_path_validity(path))
			change_path(path, env, shell, opt);
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

static void	get_cd_opt(char ***av, t_cd *opt)
{
	opt->is_oldpwd = 0;
	opt->is_p = 0;
	opt->is_l = 0;
	while (**av && ***av == '-')
	{
		opt->is_oldpwd = !(*(**av + 1)) ? 1 : opt->is_oldpwd;
		opt->is_p = *(**av + 1) == 'P' ? 1 : opt->is_p;
		opt->is_l = *(**av + 1) == 'L' ? 1 : opt->is_l;
		++*av;
	}
}

static void	cd2(char **av, t_env *env, t_sh *shell, t_cd *opt)
{
	if (cd_path_validity(*av))
		change_path(*av, env, shell, opt);
	else
	{
		if (!lstat(*av, NULL))
			errexit("cd", "Not a directory.");
		else
			errexit("cd", "No such file or directory.");
	}
}

void		cd(char **av, t_env *env, t_sh *shell)
{
	char	*tmp;
	t_cd	opt;

	++av;
	get_cd_opt(&av, &opt);
	if (*av && *(av + 1))
		errexit("cd", "Too many arguments.");
	else if ((!*av && !opt.is_oldpwd) || (*av && **av == '~'))
		cd_tilde(*av, env, shell, &opt);
	else if (opt.is_oldpwd)
	{
		if (find_env(env, "OLDPWD"))
		{
			tmp = ft_strdup(find_env(env, "OLDPWD"));
			change_path(tmp, env, shell, &opt);
			free(tmp);
		}
		else
			errexit("cd", "OLDPWD not set.");
	}
	else if (*av[0] == '/')
		cd2(av, env, shell, &opt);
	else
		cd_current_dir(*av, env, shell, &opt);
}
