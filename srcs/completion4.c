/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   completion4.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/27 15:59:22 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/12/09 19:08:46 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

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

	i = shell->j + 1;
	while (--i > -1 && shell->command[i] != ' ' && shell->command[i] != ';')
		;
	while (i > -1 && shell->command[i] == ' ')
		--i;
	if ((i == -1 && (shell->command[0] != '/' && shell->command[0] != '.')) || \
		(shell->command[i] == ';' && shell->command[i + 1] != '/' && \
		shell->command[i + 1] != '.'))
		return (1);
	return (0);
}

void		slash_if_dir(t_sh *shell, char *str)
{
	if (reg_or_dir(str) == 1 && *(str + ft_strlen(str) - 1) != '/')
		add_char(shell->command, &(shell->j), shell, '/');
}

void		convert_tild(t_sh *shell, char *str)
{
	char	*home;
	size_t	home_len;
	char	buff[4097];
	char	*tmp;

	if (*str == '~' && (home = find_env(shell->env, "HOME")))
		;
	else if (*str == '.' && *(str + 1) == '/' && (home = getcwd(buff, 4097)))
		;
	else
		return ;
	home_len = ft_strlen(home);
	if (home_len > 4000)
		return ;
	tmp = ft_strdup(str);
	ft_strncpy(str, home, home_len);
	ft_strcpy(str + home_len, tmp + 1);
	free(tmp);
}

void		write_to_prompt(t_sh *shell, char *str, char *part)
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
