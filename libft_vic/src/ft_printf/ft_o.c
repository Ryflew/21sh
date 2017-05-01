/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_o.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/02 01:44:28 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/02/18 17:41:00 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../../include/libft.h"

void	ft_puto(char *str, t_printf *elem)
{
	int i;

	if ((elem->precision != -1) && ((size_t)elem->precision > ft_strlen(str)))
	{
		i = elem->precision - ft_strlen(str);
		while (i--)
			ft_putchar('0');
	}
	ft_putstr(str);
}

int		check_complet_charo(char *nbr, char letter, t_printf *elem)
{
	int	tmp;
	int count;

	if (ft_strlen(nbr) < (size_t)(elem->width))
	{
		if ((elem->precision != -1) &&
			((size_t)elem->precision > ft_strlen(nbr)))
			tmp = elem->width - elem->precision;
		else
			tmp = elem->width - ft_strlen(nbr);
		if (elem->flag_sharp)
			tmp--;
		while (tmp-- > 0)
			ft_putchar(letter);
		if (elem->precision < elem->width)
			count = elem->width;
		else
			count = ft_strlen(nbr);
	}
	else
		count = ft_strlen(nbr);
	return (count);
}

void	putzero_ifsharp(char *nbr, t_printf *elem)
{
	if (elem->flag_sharp && !(ft_strlen(nbr) == 1 && nbr[0] == '0') &&
		elem->precision < (int)ft_strlen(nbr))
		ft_putchar('0');
}

int		with_widtho(char *nbr, t_printf *elem)
{
	int count;

	if (elem->flag_minus)
	{
		putzero_ifsharp(nbr, elem);
		ft_puto(nbr, elem);
		count = check_complet_charo(nbr, ' ', elem);
	}
	else if (elem->flag_zero)
	{
		count = check_complet_charo(nbr, '0', elem);
		putzero_ifsharp(nbr, elem);
		ft_puto(nbr, elem);
	}
	else
	{
		count = check_complet_charo(nbr, ' ', elem);
		putzero_ifsharp(nbr, elem);
		ft_puto(nbr, elem);
	}
	return (count);
}

int		ft_o(unsigned long nbr, t_printf *elem)
{
	char	*str;
	int		count;

	if (!nbr && !elem->precision && !elem->flag_sharp)
	{
		count = elem->width > 0 ? elem->width : 0;
		while (elem->width-- > 0)
			ft_putchar(' ');
		return (count);
	}
	str = ft_itoabaseu(nbr, 8, 0);
	if (elem->width)
		count = with_widtho(str, elem);
	else
	{
		putzero_ifsharp(str, elem);
		ft_puto(str, elem);
		count = ft_strlen(str);
	}
	count = end_o(str, count, elem);
	free(str);
	return (count);
}
