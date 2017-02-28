/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_oux.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/30 23:52:21 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/02/18 17:41:03 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/libft.h"

int		end_o(char *str, int count, t_printf *elem)
{
	if (elem->precision != -1 && (size_t)elem->precision > ft_strlen(str))
		count += elem->precision - ft_strlen(str);
	if (elem->flag_sharp && str[0] != '0' &&
		elem->precision < (int)ft_strlen(str))
		count++;
	if ((size_t)elem->width > ft_strlen(str) &&
		elem->precision < elem->width)
		count = elem->width;
	return (count);
}

int		check_complet_charu(unsigned long nbr, int i, char l, t_printf *elem)
{
	int	tmp;

	if (ft_countunbr(nbr) < (elem->width + i))
	{
		if (elem->precision > ft_countunbr(nbr))
			tmp = elem->width - i - elem->precision;
		else
			tmp = elem->width - i - ft_countunbr(nbr);
		while (tmp-- > 0)
			ft_putchar(l);
		i = elem->width;
	}
	else
		i += ft_countunbr(nbr);
	return (i);
}

int		with_widthu(unsigned long nbr, t_printf *elem)
{
	int count;

	count = 0;
	if (elem->flag_minus)
	{
		ft_putunbrwp(nbr, elem->precision);
		count = check_complet_charu(nbr, count, ' ', elem);
	}
	else if (elem->flag_zero)
	{
		count = check_complet_charu(nbr, count, '0', elem);
		ft_putunbrwp(nbr, elem->precision);
	}
	else
	{
		count += check_complet_charu(nbr, count, ' ', elem);
		ft_putunbrwp(nbr, elem->precision);
	}
	return (count);
}

int		ft_u(unsigned long nbr, t_printf *elem)
{
	int count;

	count = 0;
	if (!elem->precision && !nbr)
		return (0);
	if (elem->width)
		count = with_widthu(nbr, elem);
	else
	{
		count = ft_countunbr(nbr);
		ft_putunbrwp(nbr, elem->precision);
		if (elem->precision >= ft_countunbr(nbr))
			count++;
	}
	if (elem->precision > ft_countunbr(nbr))
	{
		if (elem->width > elem->precision)
			count = elem->width;
		else
			count = elem->precision;
	}
	return (count);
}

int		ft_oux(unsigned long nbr, char letter, t_printf *elem)
{
	if (letter == 'o' || letter == 'O')
		return (ft_o(nbr, elem));
	else if (letter == 'u' || letter == 'U')
		return (ft_u(nbr, elem));
	else
		return (ft_x(nbr, letter, elem));
	return (0);
}
