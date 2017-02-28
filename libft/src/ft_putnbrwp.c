/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbrwp.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/28 00:49:39 by vdarmaya          #+#    #+#             */
/*   Updated: 2016/12/14 14:38:45 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

void	ft_putnbrwp(long nbr, int precision)
{
	short count;

	if (precision > 0)
	{
		if (nbr < 0)
		{
			ft_putchar('-');
			nbr = -nbr;
		}
		count = ft_countnbr(nbr);
		count = precision - count;
		while (count-- > 0)
			ft_putchar('0');
	}
	if (nbr || precision)
		ft_putnbr(nbr);
}
