/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/21 23:14:28 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/03/22 03:39:37 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "21sh.h"

static int  get_number(char *str, int *i)
{
    int     old;
    char    *tmp;

    old = *i;
    while (str[*i] && ft_isdigit(str[*i]))
        ++(*i);
    tmp = ft_strsub(str, old, *i - old);
    old = ft_atoi(tmp);
    free(tmp);
    return (old);
}

void	get_cursor(t_sh *shell)
{
    int     i;
    int     bytes;
    char    buff[20];

    ft_putstr("\x1B[6n");
    if ((bytes = read(0, buff, 20)))
    {
        buff[bytes] = '\0';
        i = 2;
        shell->cursor.y = get_number(buff, &i) - 1;
        ++i;
        shell->cursor.x = get_number(buff, &i) - 1;
    }
}