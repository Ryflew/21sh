/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_l.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/30 15:26:01 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/02/18 17:40:57 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/libft.h"

char	check_plus_spacel(long nbr, t_printf *elem)
{
	char	count;

	count = 0;
	if (elem->flag_plus && nbr >= 0)
	{
		ft_putchar('+');
		count++;
	}
	else if (elem->flag_space && nbr >= 0)
	{
		ft_putchar(' ');
		count++;
	}
	return (count);
}

int		check_complet_charl(long nbr, int count, char letter, t_printf *elem)
{
	int	tmp;

	if (ft_countnbr(nbr) < (elem->width + count))
	{
		if (letter == ' ' && elem->flag_space && elem->flag_sharp &&
			!elem->flag_minus)
			elem->width--;
		if (elem->precision >= ft_countnbr(nbr))
		{
			tmp = elem->width - count - elem->precision;
			if (nbr < 0)
				tmp--;
		}
		else
			tmp = elem->width - count - ft_countnbr(nbr);
		while (tmp-- > 0)
			ft_putchar(letter);
		return (elem->width);
	}
	else
		count += ft_countnbr(nbr);
	return (count);
}

int		with_width_zero(long nbr, t_printf *elem)
{
	int count;

	count = 0;
	if (elem->precision != -1)
		count = check_complet_charl(nbr, count, ' ', elem);
	count += check_plus_spacel(nbr, elem);
	if (nbr < 0)
		ft_putchar('-');
	if (elem->precision == -1)
		count = check_complet_charl(nbr, count, '0', elem);
	if (nbr < 0)
		nbr = -nbr;
	ft_putnbrwp(nbr, elem->precision);
	return (count);
}

int		with_widthl(long nbr, t_printf *elem)
{
	int count;

	count = 0;
	if (elem->flag_minus)
	{
		count += check_plus_spacel(nbr, elem);
		ft_putnbrwp(nbr, elem->precision);
		count = check_complet_charl(nbr, count, ' ', elem);
	}
	else if (elem->flag_zero)
		count = with_width_zero(nbr, elem);
	else
	{
		if ((elem->flag_plus || elem->flag_space) && nbr >= 0)
			count++;
		count = check_complet_charl(nbr, count, ' ', elem);
		count += check_plus_spacel(nbr, elem);
		if ((elem->flag_plus || elem->flag_minus) && nbr >= 0)
			count--;
		ft_putnbrwp(nbr, elem->precision);
	}
	return (count);
}

int		ft_l(long nbr, t_printf *elem)
{
	long	count;

	if (!nbr && !elem->precision)
	{
		count = elem->width > 0 ? elem->width : 0;
		while (elem->width-- > 0)
			ft_putchar(' ');
		return (count);
	}
	if (elem->width)
		count = with_widthl(nbr, elem);
	else
	{
		count = check_plus_spacel(nbr, elem) + ft_countnbr(nbr);
		ft_putnbrwp(nbr, elem->precision);
		if (elem->precision >= ft_countnbr(nbr))
			count++;
	}
	if (nbr && elem->precision > ft_countnbr(nbr))
	{
		count = (nbr < 0) ? elem->precision + 1 : elem->precision;
		count = (elem->width > elem->precision) ? elem->width : count;
	}
	return (count);
}
