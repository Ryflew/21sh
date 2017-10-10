/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   completion2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/10 20:23:45 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/10/10 20:23:46 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

char	*check_dir_content(char *part, char *path)
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

char	reg_or_dir(char *path)
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

char	*get_start_str(t_sh *shell)
{
	int		end;
	int		i;

	i = shell->j + 1;
	end = i;
	while (--i > -1 && shell->command[i] != ' ')
		;
	if (i == shell->j)
		return (NULL);
	return (ft_strsub(shell->command, i + 1, end - (i + 1)));
}

void	tild_to_home(char **str, t_env *env)
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

char	*find_builtins(char *part)
{
	if (!ft_strncmp("echo", part, ft_strlen(part)))
		return (ft_strdup("echo"));
	else if (!ft_strncmp("cd", part, ft_strlen(part)))
		return (ft_strdup("cd"));
	else if (!ft_strncmp("exit", part, ft_strlen(part)))
		return (ft_strdup("exit"));
	else if (!ft_strncmp("env", part, ft_strlen(part)))
		return (ft_strdup("env"));
	else if (!ft_strncmp("setenv", part, ft_strlen(part)))
		return (ft_strdup("setenv"));
	else if (!ft_strncmp("unsetenv", part, ft_strlen(part)))
		return (ft_strdup("unsetenv"));
	else if (!ft_strncmp("hash", part, ft_strlen(part)))
		return (ft_strdup("hash"));
	else if (!ft_strncmp("help", part, ft_strlen(part)))
		return (ft_strdup("help"));
	else if (!ft_strncmp("unset", part, ft_strlen(part)))
		return (ft_strdup("unset"));
	else if (!ft_strncmp("export", part, ft_strlen(part)))
		return (ft_strdup("export"));
	else if (!ft_strncmp("history", part, ft_strlen(part)))
		return (ft_strdup("history"));
	else if (!ft_strncmp("read", part, ft_strlen(part)))
		return (ft_strdup("read"));
	return (NULL);
}
