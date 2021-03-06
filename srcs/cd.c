/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/10 20:23:24 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/09/30 19:19:54 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "tosh.h"

static void	change_path(char *path, t_env *env, t_sh *shell, t_cd *opt)
{
	char	**av;
	char	buff[4097];
	char	*tmp;
	char	*new_prompt;

	if (!is_valid_dir(path) || (new_prompt = NULL))
		return ;
	if ((tmp = find_env(get_shell()->env, "PWD")) && cd_path_validity(tmp))
		init_setenv(&av, &env, tmp);
	if (!(opt->is_logical && (tmp = find_env(get_shell()->env, "PWD"))))
		tmp = getcwd(buff, 4097);
	tmp = ft_strdup(path);
	change_prompt(tmp, env, &new_prompt, opt);
	free(tmp);
	if (!(av = (char**)malloc(sizeof(char*) * 4)))
		ft_exiterror("Malloc failed", 1);
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

static char	get_cd_opt(char ***av, t_cd *opt)
{
	opt->is_oldpwd = 0;
	opt->is_logical = 1;
	while (**av && ***av == '-')
	{
		opt->is_oldpwd = !(*(**av + 1)) ? 1 : opt->is_oldpwd;
		opt->is_logical = *(**av + 1) == 'P' ? 0 : opt->is_logical;
		opt->is_logical = *(**av + 1) == 'L' ? 1 : opt->is_logical;
		if (*(**av + 1) && *(**av + 1) != 'P' && *(**av + 1) != 'L')
		{
			ft_fputstr("cd: -", 2);
			ft_fputchar(*(**av + 1), 2);
			ft_fputstr(": invalid option", 2);
			ft_fputchar('\n', 2);
			get_shell()->return_value = 1;
			get_shell()->have_write_error = 1;
			return (0);
		}
		++*av;
	}
	return (1);
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
	if (!get_cd_opt(&av, &opt))
		return ;
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
