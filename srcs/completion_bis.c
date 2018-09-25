/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   completion_bis.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/18 18:06:09 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/09/25 19:19:18 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void	get_start_str(t_sh *shell, char *buff)
{
	int		i;

	i = shell->j + 1;
	while (--i > -1 && shell->command[i] != ' ' && shell->command[i] != ';')
		;
	if (i == shell->j)
		buff[0] = '\0';
	else
	{
		++i;
		ft_strncpy(buff, shell->command + i, (shell->j + 1) - i);
		buff[(shell->j + 1) - i] = '\0';
	}
}

static char	reg_or_dir(char *path)
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

char		is_binary_dir(t_sh *shell)
{
	int		i;

	if (shell->command[shell->j] == ';') // a test "ls;"
		return (1);
	i = shell->j + 1;
	while (--i > -1 && shell->command[i] != ' ')
		;
	if (i == -1)
		return (1);
	while (--i > -1 && shell->command[i] == ' ')
		;
	if (i == -1 || shell->command[i] == ';')
		return (1);
	return (0);
}

static char	**create_print_tab(char *path, char *part, int nb)
{
	DIR			*dir;
	t_dirent	*ent;
	char		**tab;
	int			i;

	if (!(dir = opendir(path)))
		return (NULL);
	if (!(tab = malloc(nb * sizeof(char**) + 1)))
		return (NULL);
	i = -1;
	while ((ent = readdir(dir)))
	{
		if ((!part && ft_strcmp(ent->d_name, ".") && ft_strcmp(ent->d_name, \
			"..")) || (part && !ft_strncmp(ent->d_name, part, ft_strlen(part))))
			{
				if (ent->d_type == DT_DIR)
				{
					tab[++i] = ft_memalloc(ft_strlen(ent->d_name) + 2);
					ft_strcpy(tab[i], ent->d_name);
					tab[i][ft_strlen(ent->d_name)] = '/';
				}
				else
					tab[++i] = ft_strdup(ent->d_name);
			}
	}
	tab[++i] = NULL;
	closedir(dir);
	return (tab);
}

static char	**create_bin_tab(char **path, char *part, int nb)
{
	DIR			*dir;
	t_dirent	*ent;
	char		**tab;
	int			i;
	int			count;

	if (!(tab = malloc(nb * sizeof(char**) + 1)))
		return (NULL);
	count = -1;
	i = -1;
	while (path[++count])
	{
		if (!(dir = opendir(path[count])))
		{
			tab[++count] = NULL;
			ft_strdelpp(&tab);
			return (NULL);
		}
		while ((ent = readdir(dir)))
		{
			if ((!part && ft_strcmp(ent->d_name, ".") && ft_strcmp(ent->d_name, \
				"..")) || (part && !ft_strncmp(ent->d_name, part, ft_strlen(part))))
				{
					if (ent->d_type == DT_DIR)
					{
						tab[++i] = ft_memalloc(ft_strlen(ent->d_name) + 2);
						ft_strcpy(tab[i], ent->d_name);
						tab[i][ft_strlen(ent->d_name)] = '/';
					}
					else
						tab[++i] = ft_strdup(ent->d_name);
				}
		}
		closedir(dir);
	}
	tab[++i] = NULL;
	return (tab);
}

static void	print_in_dir(char **env_path, char *path, char *part)
{
	int			obj_len;
	int			tab_count;
	char		white_space[MAX_CMD];
	char		**to_print;

	if (!env_path)
	{
		if ((obj_len = dir_content_len(path, part, &tab_count)) == -1)
			return ;
		if (!(to_print = create_print_tab(path, part, tab_count)))
			return ;
	}
	else
	{
		if ((obj_len = bin_content_len(env_path, part, &tab_count)) == -1)
			return ;
		if (!(to_print = create_bin_tab(env_path, part, tab_count)))
			return ;
	}
	ft_sorttab(&to_print);
	ft_memset(white_space, ' ', MAX_CMD);
	completion_print(to_print, white_space, obj_len);
	ft_strdelpp(&to_print);
}

static void	bin_completion_print(t_sh *shell, char *part)
{
	char	*content;
	char	**tmp;

	if (!(content = find_env(shell->env, "PATH")))
		return ;
	tmp = ft_strsplit(content, ':');
	ft_putchar('\n');
	print_in_dir(tmp, NULL, part);
	ft_strdelpp(&tmp);
}

static void	dir_completion_print(char *part)
{
	char	*path;
	char	buff[4097];
	char	*slash;

	ft_putchar('\n');
	if (!part && (path = getcwd(buff, 4097)))
		print_in_dir(NULL, path, NULL);
	else
	{
		if (!ft_strcmp(part, "."))
			print_in_dir(NULL, ".", ".");
		else
		{
			if (!(slash = ft_strrchr(part, '/')))
				print_in_dir(NULL, ".", part);
			else
			{
				ft_strncpy(buff, part, slash - part + 1);
				buff[slash - part + 1] = '\0';
				print_in_dir(NULL, buff, !*(slash + 1) ? NULL : slash + 1);
			}
		}
	}
}

static void		slash_if_dir(t_sh *shell, char *str)
{
	if (reg_or_dir(str) == 1 && *(str + ft_strlen(str) - 1) != '/')
		add_char(shell->command, &(shell->j), shell, '/');
}

static void	convert_tild(t_sh *shell, char *str)
{
	char	*home;
	size_t	home_len;

	if (*str != '~' || !(home = find_env(shell->env, "HOME")))
		return ;
	home_len = ft_strlen(home);
	if (home_len > 4000)
		return ;
	ft_strcpy(str + home_len, str + 1);
	ft_strncpy(str, home, home_len);
}

static void		write_to_prompt(t_sh *shell, char *str, char *part)
{
	int		i;
	int		len;

	i = -1;
	len = ft_strlen(str);
	while (++i < len)
		add_char(shell->command, &(shell->j), shell, *(str + i));
	if (part)
	{
		get_start_str(shell, part);
		convert_tild(shell, part);
		slash_if_dir(shell, part);
	}
}

void		go_completion(t_sh *shell)
{
	char	part[MAX_CMD];
	char	save_cmd[MAX_CMD];
	int		need_print;
	char	to_add[MAX_CMD];
	char	is_binary;

	shell->command[shell->j + 1] = '\0';
	to_add[0] = '\0';
	get_start_str(shell, part);
	is_binary = is_binary_dir(shell);
	if (!part[0])
	{
		if (is_binary && !find_env(shell->env, "PATH"))
			return ;
		is_binary ? bin_completion_print(shell, NULL) : dir_completion_print(NULL);
		save_cmd[0] = '\0';
		if (shell->j > -1)
			ft_strcpy(save_cmd, shell->command);
		get_cursor(shell);
		sig_hand(0);
		if (save_cmd[0])
			write_to_prompt(shell, save_cmd, NULL);
	}
	else
	{
		convert_tild(shell, part);
		need_print = is_binary ? get_bin_occur(shell, part, to_add) : get_dir_occur(part, to_add);
		if (to_add[0])
			write_to_prompt(shell, to_add, need_print ? NULL : part);
		else if (need_print)
		{
			ft_strcpy(save_cmd, shell->command);
			is_binary ? bin_completion_print(shell, part) : dir_completion_print(part);
			get_cursor(shell);
			sig_hand(0);
			ft_strcpy(save_cmd + ft_strlen(save_cmd), to_add);
			write_to_prompt(shell, save_cmd, NULL);
		}
		else
			slash_if_dir(shell, part);
	}
}
