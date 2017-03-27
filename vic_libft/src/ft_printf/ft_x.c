/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_x.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/01 21:36:14 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/02/18 17:41:25 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../../include/libft.h"

void	ft_putx(char *str, t_printf *elem)
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

int		check_complet_charx(char *nbr, char letter, t_printf *elem)
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
			tmp -= 2;
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

int		with_widthx(char *nbr, char letter, t_printf *elem)
{
	int count;

	if (elem->flag_minus)
	{
		if (elem->flag_sharp && !(ft_strlen(nbr) == 1 && nbr[0] == '0'))
			ft_putzerox(letter);
		ft_putx(nbr, elem);
		count = check_complet_charx(nbr, ' ', elem);
	}
	else if (elem->flag_zero)
	{
		if (elem->flag_sharp && !(ft_strlen(nbr) == 1 && nbr[0] == '0'))
			ft_putzerox(letter);
		count = check_complet_charx(nbr, '0', elem);
		ft_putx(nbr, elem);
	}
	else
	{
		count = check_complet_charx(nbr, ' ', elem);
		if (elem->flag_sharp && !(ft_strlen(nbr) == 1 && nbr[0] == '0'))
			ft_putzerox(letter);
		ft_putx(nbr, elem);
	}
	return (count);
}

int		without_width(char *str, char letter, t_printf *elem)
{
	if (elem->flag_sharp && !(ft_strlen(str) == 1 && str[0] == '0'))
		ft_putzerox(letter);
	ft_putx(str, elem);
	return (ft_strlen(str));
}

int		ft_x(unsigned long nbr, char letter, t_printf *elem)
{
	char	*str;
	int		count;

	if (!nbr && !elem->precision)
	{
		count = elem->width;
		while (elem->width-- > 0)
			ft_putchar(' ');
		if (count)
			return (count);
		return (0);
	}
	str = (letter == 'x') ? ft_itoabaseu(nbr, 16, 1) : ft_itoabaseu(nbr, 16, 0);
	if (elem->width)
		count = with_widthx(str, letter, elem);
	else
		count = without_width(str, letter, elem);
	if ((elem->precision != -1) && ((size_t)elem->precision > ft_strlen(str)))
		count += elem->precision - ft_strlen(str);
	if (elem->flag_sharp && !(ft_strlen(str) == 1 && str[0] == '0'))
		count += 2;
	if ((size_t)elem->width > ft_strlen(str) && elem->precision < elem->width)
		count = elem->width;
	free(str);
	return (count);
}
