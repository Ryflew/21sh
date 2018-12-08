/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   completion2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/20 13:06:14 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/12/08 17:23:30 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

static void	check_dir_content2(char *to_add, char *fill, t_dirent *ent, \
	size_t part_len)
{
	size_t		i;

	if (!*to_add && *fill)
		ft_strcpy(to_add, ent->d_name + part_len);
	else
	{
		i = 0;
		while (i < ft_strlen(to_add) && *(ent->d_name + i))
		{
			if (*(to_add + i) != *(ent->d_name + part_len + i))
			{
				ft_bzero(to_add + i, ft_strlen(to_add) - i);
				if (!to_add[0])
					*fill = 0;
				break ;
			}
			++i;
		}
	}
}

static char	check_dir_content(char *path, char *part, char *to_add, char *fill)
{
	DIR			*dir;
	t_dirent	*ent;
	char		out;
	size_t		part_len;

	if (!(dir = opendir(path)))
		return (0);
	part_len = ft_strlen(part);
	out = 0;
	while ((ent = readdir(dir)))
	{
		if (!ft_strncmp(ent->d_name, part, part_len))
		{
			if (!ft_strcmp(part, ent->d_name) && !(*fill = 0))
				to_add[0] = '\0';
			else
				check_dir_content2(to_add, fill, ent, part_len);
			if (out < 2)
				++out;
		}
	}
	closedir(dir);
	return (out);
}

int		check_builtin(char *part, char *to_add)
{
	char	*arr[8];
	int		out;

	arr[0] = "setenv";
	arr[1] = "unsetenv";
	arr[2] = "exit";
	arr[3] = "history";
	arr[4] = "export";
	arr[5] = "shellvar";
	arr[6] = "help";
	arr[7] = "unset";
	out = 0;
	if (!ft_strncmp(part, arr[0], ft_strlen(part)) && ++out)
		ft_strcpy(to_add, arr[0] + ft_strlen(part));
	if (!ft_strncmp(part, arr[1], ft_strlen(part)) && ++out)
		ft_strcpy(to_add, arr[1] + ft_strlen(part));
	if (!ft_strncmp(part, arr[2], ft_strlen(part)) && ++out)
		ft_strcpy(to_add, arr[2] + ft_strlen(part));
	if (!ft_strncmp(part, arr[3], ft_strlen(part)) && ++out)
		ft_strcpy(to_add, arr[3] + ft_strlen(part));
	if (!ft_strncmp(part, arr[4], ft_strlen(part)) && ++out)
		ft_strcpy(to_add, arr[4] + ft_strlen(part));
	if (!ft_strncmp(part, arr[5], ft_strlen(part)) && ++out)
		ft_strcpy(to_add, arr[5] + ft_strlen(part));
	if (!ft_strncmp(part, arr[6], ft_strlen(part)) && ++out)
		ft_strcpy(to_add, arr[6] + ft_strlen(part));
	if (!ft_strncmp(part, arr[7], ft_strlen(part)) && ++out)
		ft_strcpy(to_add, arr[7] + ft_strlen(part));
	return (out > 1 ? 1 : 0);
}

char		get_bin_occur(t_sh *shell, char *part, char *to_add, int i)
{
	char	*content;
	char	**tmp;
	char	out;
	char	ret;
	char	fill;

	if (!(content = find_env(shell->env, "PATH")))
		return (0);
	tmp = ft_strsplit(content, ':');
	fill = 1;
	out = 0;
	while (tmp[++i])
	{
		if ((ret = check_dir_content(tmp[i], part, to_add, &fill)) == 2)
			out = 2;
		else if (ret && out < 2)
			++out;
	}
	ft_strdelpp(&tmp);
	out += check_builtin(part, to_add);
	return (out > 1 ? 1 : 0);
}

char		get_dir_occur(char *part, char *to_add, char out)
{
	char	*slash;
	char	fill;
	char	*cwd;
	char	buff[4097];
	char	path[4097];

	fill = 1;
	slash = ft_strrchr(part, '/');
	if (slash - part > 4096)
		return (0);
	if (!slash && (cwd = getcwd(buff, 4097)))
		out = check_dir_content(cwd, part, to_add, &fill);
	else
	{
		ft_strncpy(path, part, slash - part + 1);
		path[slash - part + 1] = '\0';
		if (!*(slash + 1))
			return (1);
		out = check_dir_content(path, slash + 1, to_add, &fill);
	}
	return (out > 1 ? 1 : 0);
}
