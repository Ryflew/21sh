/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdelpp.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/11 23:50:56 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/01/24 04:05:42 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../include/libft.h"

void	ft_strdelpp(char ***str)
{
	int		i;
	char	**tmp;

	tmp = *str;
	i = -1;
	while (tmp[++i])
	{
		ft_strdel(&(tmp[i]));
		tmp[i] = NULL;
	}
	free(*str);
	*str = NULL;
}
