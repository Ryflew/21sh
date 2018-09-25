/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sorttab.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/25 15:49:10 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/09/25 17:13:47 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "string.h"

void	ft_sorttab(char ***tabu)
{
	char	**tmp;
	char	**tmp2;
	char	*save;

	tmp = *tabu;
	while (*tmp && *(tmp + 1))
	{
		tmp2 = *tabu;
		while (*tmp2 && *(tmp2 + 1))
		{
			if (ft_strcmp(*tmp2, *(tmp2 + 1)) > 0)
			{
				save = *tmp2;
				*tmp2 = *(tmp2 + 1);
				*(tmp2 + 1) = save;
			}
			++tmp2;
		}
		++tmp;
	}
}
