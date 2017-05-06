/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd2.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/18 22:54:35 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/05/06 19:43:13 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "21sh.h"

static void	treat_current2(void)
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
		chdir(tmp);
		free(tmp);
	}
}

static void	treat_current(char *path)
{
	char	*tmp;
	char	*cwd;
	char	buff[4097];

	if (!ft_strcmp(path, "."))
		return ;
	else if (!ft_strcmp(path, ".."))
		treat_current2();
	else if (*path == '/')
		chdir(path);
	else
	{
		cwd = getcwd(buff, 4097);
		if (!ft_strcmp(cwd, "/"))
			tmp = ft_strjoin(cwd, path);
		else
			tmp = ft_strstrjoin(cwd, "/", path);
		if (!access(tmp, R_OK | R_OK))
			chdir(tmp);
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

void	change_prompt(char *path, t_env *env)
{
	char	*tmp;
	char	*path2;

	if (!ft_strchr(path, '/') || !ft_strcmp(path, "/"))
		treat_current(path);
	else
	{
		if (*path == '/' && !ft_strchr(path + 1, '/'))
		{
			tmp = ft_strsub(path, 0, 1);
			path2 = ft_strsub(path, 1, ft_strlen(path) - 1);
		}
		else
			change_prompt2(&path2, path, &tmp);
		treat_current(tmp);
		free(tmp);
		if (ft_strlen(path2) > 0)
			change_prompt(path2, env);
		free(path2);
	}
}

char	*get_with_tilde(char *path, t_env *env)
{
	char	*home;
	char	*tmp;
	int		i;

	i = -1;
	if (!(home = find_env(env, "HOME")))
		return (ft_strdup(path));
	while (*home && path[++i] && (path[i] == *home))
		++home;
	if (*home)
		return (ft_strdup(path));
	if (!path[i + 1])
		return (ft_strdup("~"));
	home = ft_strsub(path + i + 1, 0, ft_strlen(path) - i - 1);
	tmp = ft_strjoin("~", home);
	free(home);
	return (tmp);
}
