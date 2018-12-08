/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   completion_print.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/25 14:50:35 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/12/03 15:31:34 by vdarmaya         ###   ########.fr       */
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
		if (x + file_len + obj_len - file_len + 1 >= \
			get_shell()->pos.max_window.x)
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

static int	bin_content_len(char **path, char *p, int *tab_count, int i)
{
	DIR			*dir;
	t_dirent	*ent;
	int			size;
	char		is_dir;

	*tab_count = 0;
	size = 2;
	while (path[++i])
	{
		if (!(dir = opendir(path[i])))
			return (-1);
		while ((ent = readdir(dir)))
		{
			if ((!p && ft_strcmp(ent->d_name, ".") && ft_strcmp(ent->d_name, \
				"..")) || (p && !ft_strncmp(ent->d_name, p, ft_strlen(p))))
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

static void	display_possibilities(int tab_count)
{
	get_shell()->tab_request = 1;
	ft_putstr("Display all ");
	ft_putnbr(tab_count);
	ft_putstr(" possibilities? (y or n)");
}

char		print_in_dir(char **env_path, char *path, char *part, char c)
{
	int			obj_len;
	int			tab_count;
	char		white_space[MAX_CMD];
	char		**to_print;

	if ((obj_len = !env_path ? dir_content_len(path, part, &tab_count) : \
		bin_content_len(env_path, part, &tab_count, -1)) == -1)
		return (0);
	if (tab_count > 99 && !get_shell()->tab_request)
	{
		display_possibilities(tab_count);
		return (1);
	}
	else if (tab_count > 99 && get_shell()->tab_request && c == 'n')
		return (0);
	if (!(to_print = !env_path ? create_print_tab(path, part, tab_count, -1) : \
		create_bin_tab(env_path, part, tab_count)))
		return (0);
	get_shell()->tab_request = 0;
	ft_sorttab(&to_print);
	ft_memset(white_space, ' ', MAX_CMD);
	completion_print(to_print, white_space, obj_len);
	ft_strdelpp(&to_print);
	return (0);
}
