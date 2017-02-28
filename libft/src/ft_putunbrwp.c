/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putunbrwp.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/01 17:38:13 by vdarmaya          #+#    #+#             */
/*   Updated: 2016/12/14 14:39:08 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

void	ft_putunbrwp(unsigned long nbr, int precision)
{
	short count;

	if (precision > 0)
	{
		count = ft_countnbr(nbr);
		count = precision - count;
		while (count-- > 0)
			ft_putchar('0');
	}
	if (nbr || precision)
		ft_putunbr(nbr);
}
