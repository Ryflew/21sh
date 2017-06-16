/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_quot.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/04 23:11:41 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/06/16 02:55:50 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "21sh.h"

static int	count_quot(char *str, char c, int i, int nb)
{
	int value;

	value = ft_strlen(str);
	while (++i < value)
	{
		if (str[i] != ' ')
		{
			while (str[i] && str[i] != ' ')
			{
				if (str[i] == '"' || str[i] == '\'' || str[i] == '`')
				{
					if (str[i] == c)
						c = 0;
					else if (!c)
						c = str[i];
				}
				++i;
			}
			if (!c)
				++nb;
		}
	}
	return (nb);
}

static void	get_next_quot(char *buff, char *cmd, int *count, int *i)
{
	char	c;

	c = cmd[*i];
	while (cmd[++(*i)])
	{
		if (cmd[*i] == c)
		{
			++(*i);
			return ;
		}
		buff[++(*count)] = cmd[*i];
	}
}

static void	get_splited_char(char *cmd, char ***out, int j, int i)
{
	int		count;
	char	buff[ft_strlen(cmd)];

	ft_bzero(buff, ft_strlen(cmd));
	count = -1;
	while (cmd[++i])
	{
		if (cmd[i] != ' ')
		{
			while (cmd[i] && cmd[i] != ' ')
			{
				if (cmd[i] == '"' || cmd[i] == '\'' || cmd[i] == '`')
					get_next_quot(buff, cmd, &count, &i);
				else
					buff[++count] = cmd[i++];
			}
			buff[++count] = '\0';
			(*out)[++j] = ft_strdup(buff);
			cmd += i;
			i = -1;
			ft_bzero(buff, ft_strlen(cmd));
			count = -1;
		}
	}
}

char		**split_quot_cmd(char *cmd)
{
	int		nb;
	char	**out;

	if (!cmd)
		return (NULL);
	nb = count_quot(cmd, 0, -1, 0);
	if (!(out = (char**)malloc(sizeof(char*) * nb + 1)))
	{
		ft_putstr("Malloc failed.");
		return (NULL);
	}
	out[nb] = NULL;
	get_splited_char(cmd, &out, -1, -1);
	return (out);
}
