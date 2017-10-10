/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   completion.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/10 20:23:40 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/10/10 20:23:41 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include "tosh.h"

static void	print_missing2(char **str, char **part, char *all)
{
	char	*tmp;
	char	buff[4097];

	tmp = ft_strsub(*part, 0, ft_strrchr(*part, '/') - *part);
	*str = ft_strstrjoin(tmp, "/", all);
	free(tmp);
	if (**part != '/')
	{
		tmp = *str;
		*str = ft_strstrjoin(getcwd(buff, 4097), "/", tmp);
		free(tmp);
	}
	*part = ft_strrchr(*part, '/') + 1;
}

static void	print_missing(t_sh *shell, char *part, char *all)
{
	int		i;
	char	buff[4097];
	char	*cwd;
	char	*str;

	i = -1;
	if (!(cwd = getcwd(buff, 4097)))
		return ;
	if (ft_strchr(part, '/'))
		print_missing2(&str, &part, all);
	else
		str = ft_strstrjoin(cwd, "/", all);
	while (part[++i] && all[i] && part[i] == all[i])
		;
	while (all[i])
		add_char(shell->command, &(shell->j), shell, all[i++]);
	if ((i = reg_or_dir(str)) == 1)
		add_char(shell->command, &(shell->j), shell, '/');
	else if (i == 2)
		add_char(shell->command, &(shell->j), shell, ' ');
	free(str);
}

static void	find_match_elem2(char *path, char **tmp, char **tmp2, char **cwd)
{
	if (!*tmp)
		*tmp = path;
	*tmp2 = ft_strsub(path, 0, ft_strlen(path) - ft_strlen(*tmp));
	*cwd = ft_strstrjoin(*cwd, "/", *tmp2);
	free(*tmp2);
	if (**tmp == '/')
		++(*tmp);
}

static char	*find_match_elem(char *path, char *cwd, char *tmp, char *tmp2)
{
	char	buff[4097];

	if (*path == '/')
	{
		tmp = ft_strrchr(path, '/');
		if ((!tmp || !*(tmp + 1)) && ft_strchr(path, '/'))
			return (NULL);
		cwd = ft_strsub(path, 0, ft_strlen(path) - ft_strlen(++tmp));
	}
	else
	{
		if (!(cwd = getcwd(buff, 4097)))
			return (NULL);
		tmp = ft_strrchr(path, '/');
		if ((!tmp || !*(tmp + 1)) && ft_strchr(path, '/'))
			return (NULL);
		find_match_elem2(path, &tmp, &tmp2, &cwd);
	}
	if ((tmp2 = check_dir_content(tmp, cwd)))
	{
		free(cwd);
		return (tmp2);
	}
	free(cwd);
	return (NULL);
}

void		go_completion(t_sh *shell)
{
	char	*tmp;
	char	*name;

	shell->command[shell->j + 1] = '\0';
	if (!(tmp = get_start_str(shell)))
		return ;
	if (*tmp == '~')
		tild_to_home(&tmp, shell->env);
	if (*tmp == '.' && *(tmp + 1) && *(tmp + 1) == '/')
		current_completion(&tmp);
	if (!tmp)
		return ;
	if (ft_strchr(shell->command, ' ') || *tmp == '/')
		name = find_match_elem(tmp, NULL, NULL, NULL);
	else
		name = find_match_binary(shell, tmp);
	if (!name)
		name = find_builtins(tmp);
	if (name)
	{
		print_missing(shell, tmp, name);
		free(name);
	}
	free(tmp);
}
