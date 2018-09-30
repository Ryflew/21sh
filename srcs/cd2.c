/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd2.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/18 22:54:35 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/09/30 19:32:29 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "tosh.h"

static void	treat_current2(t_cd *opt, char *tmp, char **new_prompt)
{
	char	buff[4097];

	if (!opt->is_logical && !access(tmp, R_OK | R_OK))
	{
		chdir(tmp);
		rewrite_env(get_shell()->env, "PWD", tmp);
		ft_strdel(new_prompt);
		*new_prompt = ft_strdup(getcwd(buff, 4097));
	}
	else if (!access(tmp, R_OK | R_OK))
	{
		ft_strdel(new_prompt);
		*new_prompt = ft_strdup(tmp);
		chdir(tmp);
		rewrite_env(get_shell()->env, "PWD", tmp);
	}
	else
		errexit("cd", "permission denied.");
}

static void	treat_current3(char **new_prompt, t_cd *opt, char *path)
{
	char	*tmp;
	char	*cwd;
	char	buff[4097];

	if (!(opt->is_logical && (cwd = find_env(get_shell()->env, "PWD"))))
		cwd = getcwd(buff, 4097);
	if (!ft_strcmp(cwd, "/"))
		tmp = ft_strjoin(cwd, path);
	else
		tmp = ft_strstrjoin(cwd, "/", path);
	treat_current2(opt, tmp, new_prompt);
	free(tmp);
}

static void	treat_current(char *path, char **new_prompt, t_cd *opt)
{
	char	*cwd;
	char	buff[4097];

	if (!ft_strcmp(path, "."))
	{
		ft_strdel(new_prompt);
		if (!(opt->is_logical && (cwd = find_env(get_shell()->env, "PWD"))))
			*new_prompt = ft_strdup(getcwd(buff, 4097));
		else
			*new_prompt = ft_strdup(cwd);
		return ;
	}
	else if (!ft_strcmp(path, ".."))
		cd_dot_dot(new_prompt, opt);
	else if (*path == '/')
	{
		chdir(path);
		if (!opt->is_logical)
			path = getcwd(buff, 4097);
		rewrite_env(get_shell()->env, "PWD", path);
		ft_strdel(new_prompt);
		*new_prompt = ft_strdup(path);
	}
	else
		treat_current3(new_prompt, opt, path);
}

static void	change_prompt2(char **path2, char *path, char **tmp)
{
	int		i;

	if (*path == '/')
		i = ft_strchr(path + 1, '/') - path;
	else
		i = ft_strchr(path, '/') - path;
	*tmp = ft_strsub(path, 0, i);
	*path2 = ft_strsub(path, i + 1, ft_strlen(path) - i - 1);
}

void		change_prompt(char *path, t_env *env, char **new_promp, t_cd *opt)
{
	char	*tmp;
	char	*path2;
	char	buff[4097];

	if (!ft_strchr(path, '/') || !ft_strcmp(path, "/"))
	{
		treat_current(path, new_promp, opt);
		if (!opt->is_logical)
		{
			ft_strdel(new_promp);
			*new_promp = ft_strdup(getcwd(buff, 4097));
		}
	}
	else
	{
		if (*path == '/' && !ft_strchr(path + 1, '/') && \
			(tmp = ft_strsub(path, 0, 1)))
			path2 = ft_strsub(path, 1, ft_strlen(path) - 1);
		else
			change_prompt2(&path2, path, &tmp);
		treat_current(tmp, new_promp, opt);
		free(tmp);
		ft_strlen(path2) > 0 ? change_prompt(path2, env, new_promp, opt) : NULL;
		free(path2);
	}
}
