/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdurst <bdurst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/11 17:45:46 by bdurst            #+#    #+#             */
/*   Updated: 2017/10/11 17:45:46 by bdurst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include "tosh.h"

static char	check_path_end(char **tmp, DIR **dir, char out)
{
	free(*tmp);
	closedir(*dir);
	return (out);
}

static char	check_path2(char *tmp)
{
	t_stat		file;

	if (!stat(tmp, &file) && S_ISREG(file.st_mode) && is_binary(tmp) &&
		!access(tmp, R_OK | X_OK))
		return (1);
	else if (access(tmp, R_OK | X_OK) == -1)
	{
		errexit(tmp, "Permission denied.");
		return (2);
	}
	return (0);
}

char		check_path(char *command, char *path)
{
	DIR			*dir;
	t_dirent	*ent;
	char		*tmp;
	char		out;

	if (!(dir = opendir(path)))
		return (0);
	while ((ent = readdir(dir)))
	{
		if (!ft_strcmp(ent->d_name, command))
		{
			tmp = ft_strstrjoin(path, "/", ent->d_name);
			if ((out = check_path2(tmp)))
				return (check_path_end(&tmp, &dir, out));
			free(tmp);
		}
	}
	closedir(dir);
	return (0);
}
