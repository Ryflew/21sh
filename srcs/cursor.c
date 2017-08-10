/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/21 23:14:28 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/08/10 03:07:02 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "tosh.h"

static int	get_number(char *str, int *i)
{
	int		old;
	char	*tmp;

	old = *i;
	while (str[*i] && ft_isdigit(str[*i]))
		++(*i);
	tmp = ft_strsub(str, old, *i - old);
	old = ft_atoi(tmp);
	free(tmp);
	return (old);
}

void		get_cursor(t_sh *shell)
{
	int		i;
	int		bytes;
	char	buff[20];

	ft_putstr("\x1B[6n");
	if ((bytes = read(0, buff, 20)))
	{
		buff[bytes] = '\0';
		i = 2;
		shell->pos.cursor.y = get_number(buff, &i) - 1;
		++i;
		shell->pos.cursor.x = get_number(buff, &i) - 1;
	}
	shell->pos.first = (t_pos){shell->pos.cursor.x, shell->pos.cursor.y};
	shell->pos.last = (t_pos){shell->pos.cursor.x, shell->pos.cursor.y};
}

int			get_current_pos_in_command(t_sh *shell)
{
	if (shell->pos.first.y == shell->pos.cursor.y)
		return (shell->pos.cursor.x - shell->pos.first.x);
	else
		return ((shell->pos.max_window.x - shell->pos.first.x) + \
			((shell->pos.cursor.y - shell->pos.first.y - 1) * \
			shell->pos.max_window.x) + shell->pos.cursor.x);
}
