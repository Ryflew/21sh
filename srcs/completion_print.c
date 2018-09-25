/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   completion_print.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/25 14:50:35 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/09/25 18:39:14 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

void		completion_print(char **tabu, char *white_space, int obj_len)
{
	int		i;
	int		x;
	int		file_len;

	i = -1;
	x = 0;
	while (tabu[++i])
	{
		file_len = ft_strlen(tabu[i]);
		if (x + file_len + obj_len - file_len + 1 >= get_shell()->pos.max_window.x)
		{
			ft_putchar('\n');
			x = 0;
		}
		ft_putstr(tabu[i]);
		write(1, white_space, obj_len - file_len + 1);
		x += file_len + obj_len - file_len + 1;
	}
}

int			dir_content_len(char *path, char *part, int *tab_count)
{
	DIR			*dir;
	t_dirent	*ent;
	int			size;
	char		is_dir;

	if (!(dir = opendir(path)))
		return (-1);
	*tab_count = 0;
	size = 2;
	while ((ent = readdir(dir)))
	{
		if ((!part && ft_strcmp(ent->d_name, ".") && ft_strcmp(ent->d_name, \
			"..")) || (part && !ft_strncmp(ent->d_name, part, ft_strlen(part))))
		{
			is_dir = ent->d_type == DT_DIR ? 1 : 0;
			if (ft_strlen(ent->d_name) + is_dir > (size_t)size)
				size = (int)ft_strlen(ent->d_name) + is_dir;
			++*tab_count;
		}
	}
	closedir(dir);
	return (size);
}

int		bin_content_len(char **path, char *part, int *tab_count)
{
	DIR			*dir;
	t_dirent	*ent;
	int			size;
	char		is_dir;
	int			i;

	i = -1;
	*tab_count = 0;
	size = 2;
	while (path[++i])
	{
		if (!(dir = opendir(path[i])))
			return (-1);
		while ((ent = readdir(dir)))
		{
			if ((!part && ft_strcmp(ent->d_name, ".") && ft_strcmp(ent->d_name, \
				"..")) || (part && !ft_strncmp(ent->d_name, part, ft_strlen(part))))
			{
				is_dir = ent->d_type == DT_DIR ? 1 : 0;
				if (ft_strlen(ent->d_name) + is_dir > (size_t)size)
					size = (int)ft_strlen(ent->d_name) + is_dir;
				++*tab_count;
			}
		}
		closedir(dir);
	}
	return (size);
}
