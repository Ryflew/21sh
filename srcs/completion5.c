/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   completion5.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/27 16:01:56 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/09/27 16:02:13 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

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
