/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_countnbr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/28 01:01:28 by vdarmaya          #+#    #+#             */
/*   Updated: 2016/12/04 16:30:24 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

short	ft_countnbr(long nbr)
{
	short count;

	if (nbr == -9223372036854775807 - 1)
		return (20);
	if (!nbr)
		return (1);
	count = 0;
	if (nbr < 0)
	{
		nbr = -nbr;
		count++;
	}
	while (nbr)
	{
		nbr /= 10;
		count++;
	}
	return (count);
}
