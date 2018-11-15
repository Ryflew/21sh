/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shellvar.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/15 14:41:56 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/11/15 14:43:45 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

void	shellvar(t_env *shellvar)
{
	while (shellvar)
	{
		ft_putendl(shellvar->all);
		NEXT(shellvar);
	}
}
