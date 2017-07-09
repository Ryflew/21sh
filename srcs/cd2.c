/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd2.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/18 22:54:35 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/07/09 17:05:44 by bdurst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "21sh.h"

static void	cd_dot_dot(char **new_prompt)
{
	char	*tmp;
	char	*cwd;
	char	buff[4097];

	cwd = getcwd(buff, 4097);
	if (ft_strlen(cwd) > 1)
	{
		if ((ft_strrchr(cwd, '/') - cwd) == 0)
			tmp = ft_strdup("/");
		else
			tmp = ft_strsub(cwd, 0, ft_strrchr(cwd, '/') - cwd);
		ft_strdel(new_prompt);
		*new_prompt = ft_strdup(tmp);
		chdir(tmp);
		free(tmp);
	}
	else
		*new_prompt = ft_strdup(getcwd(buff, 4097));
}

static void	treat_current2(t_cd *opt, char *tmp, char **new_prompt)
{
	char	buff[4097];

	if (opt->is_p && !access(tmp, R_OK | R_OK))
	{
		chdir(tmp);
		ft_strdel(new_prompt);
		*new_prompt = ft_strdup(getcwd(buff, 4097));
	}
	else if (!access(tmp, R_OK | R_OK))
	{
		ft_strdel(new_prompt);
		*new_prompt = ft_strdup(tmp);
		chdir(tmp);
	}
	else
		errexit("cd", "permission denied.");
}

static void	treat_current(char *path, char **new_prompt, t_cd *opt)
{
	char	*tmp;
	char	*cwd;
	char	buff[4097];

	if (!ft_strcmp(path, "."))
		return ;
	else if (!ft_strcmp(path, ".."))
		cd_dot_dot(new_prompt);
	else if (*path == '/')
	{
		chdir(path);
		ft_strdel(new_prompt);
		*new_prompt = ft_strdup(path);
	}
	else
	{
		cwd = getcwd(buff, 4097);
		if (!ft_strcmp(cwd, "/"))
			tmp = ft_strjoin(cwd, path);
		else
			tmp = ft_strstrjoin(cwd, "/", path);
		treat_current2(opt, tmp, new_prompt);
		free(tmp);
	}
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

void		change_prompt(char *path, t_env *env, char **new_prompt, t_cd *opt)
{
	char	*tmp;
	char	*path2;

	if (!ft_strchr(path, '/') || !ft_strcmp(path, "/"))
		treat_current(path, new_prompt, opt);
	else
	{
		if (*path == '/' && !ft_strchr(path + 1, '/'))
		{
			tmp = ft_strsub(path, 0, 1);
			path2 = ft_strsub(path, 1, ft_strlen(path) - 1);
		}
		else
			change_prompt2(&path2, path, &tmp);
		treat_current(tmp, new_prompt, opt);
		free(tmp);
		if (ft_strlen(path2) > 0)
			change_prompt(path2, env, new_prompt, opt);
		free(path2);
	}
}
