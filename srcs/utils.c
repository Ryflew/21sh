/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/11 17:44:33 by bdurst            #+#    #+#             */
/*   Updated: 2018/11/29 15:05:51 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "tosh.h"

char	cd_path_validity(char *path)
{
	DIR		*dir;

	if (!path || !(dir = opendir(path)))
		return (0);
	closedir(dir);
	return (1);
}

char	is_binary(char *path)
{
	int		fd;
	char	buff[300];
	int		bytes;

	if ((fd = open(path, O_RDONLY)) == -1)
		return (0);
	if ((bytes = read(fd, buff, 299)) > 0)
	{
		while (--bytes)
		{
			if (buff[bytes] == '\0')
			{
				close(fd);
				return (1);
			}
		}
	}
	close(fd);
	return (0);
}

char	*clear_quot(char *str, char string_op)
{
	int		i;
	int		count;
	char	*out;

	i = -1;
	count = 0;
	while (str[++i])
		if (str[i] != string_op)
			++count;
	if (!(out = (char *)malloc(sizeof(char) * (count + 1))))
		ft_exiterror("Malloc failed", 1);
	i = -1;
	count = -1;
	while (str[++i])
		if (str[i] != string_op)
			out[++count] = str[i];
	out[++count] = '\0';
	free(str);
	return (out);
}

char	only_space(char *str)
{
	while (*str)
	{
		if (*str != ' ')
			return (0);
		++str;
	}
	return (1);
}

void	free_join(char **s1, char *s2)
{
	char	*to_free;

	if (*s1)
	{
		to_free = *s1;
		*s1 = ft_strjoin(*s1, s2);
		free(to_free);
	}
	else
		*s1 = ft_strdup(s2);
}
