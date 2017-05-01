/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_p.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/03 17:00:24 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/02/18 17:41:06 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../../include/libft.h"

void	ft_putp(char *str, t_printf *elem)
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

int		check_complet_charp(char *nbr, char letter, t_printf *elem)
{
	int	tmp;
	int count;

	if (ft_strlen(nbr) + 2 < (size_t)(elem->width))
	{
		tmp = elem->width - ft_strlen(nbr) - 2;
		while (tmp-- > 0)
			ft_putchar(letter);
		count = elem->width;
	}
	else
		count = ft_strlen(nbr) + 2;
	return (count);
}

int		with_widthp(char *nbr, t_printf *elem)
{
	int count;

	if (elem->flag_minus)
	{
		ft_putstr("0x");
		ft_putp(nbr, elem);
		count = check_complet_charp(nbr, ' ', elem);
	}
	else if (elem->flag_zero)
	{
		ft_putstr("0x");
		count = check_complet_charp(nbr, '0', elem);
		ft_putp(nbr, elem);
	}
	else
	{
		count = check_complet_charp(nbr, ' ', elem);
		ft_putstr("0x");
		ft_putp(nbr, elem);
	}
	return (count);
}

int		ft_p(void *address, t_printf *elem)
{
	char	*str;
	int		count;

	str = ft_itoabaseu((unsigned long)address, 16, 1);
	if (str[0] == '0' && ft_strlen(str) == 1 && !elem->precision)
	{
		count = elem->width > 0 ? elem->width : 0;
		while (elem->width-- > 0)
			ft_putchar(' ');
		ft_putstr("0x");
		return (count + 2);
	}
	if (elem->width)
		count = with_widthp(str, elem);
	else
	{
		ft_putstr("0x");
		ft_putp(str, elem);
		count = ft_strlen(str) + 2;
	}
	if ((elem->precision != -1) && ((size_t)elem->precision > ft_strlen(str)))
		count += elem->precision - ft_strlen(str);
	free(str);
	return (count);
}
