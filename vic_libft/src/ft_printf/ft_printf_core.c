/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_core.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/26 04:04:52 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/02/18 17:41:12 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <wchar.h>
#include <stdio.h>
#include "../../include/libft.h"

unsigned long	cast_lenu(va_list ap, t_printf *elem)
{
	if (!elem->length)
		return (va_arg(ap, unsigned int));
	else if (!ft_strcmp(elem->length, "l") || !ft_strcmp(elem->length, "ll") ||
				!ft_strcmp(elem->length, "j"))
		return (va_arg(ap, unsigned long));
	else if (!ft_strcmp(elem->length, "h"))
		return ((unsigned short)va_arg(ap, unsigned int));
	else if (!ft_strcmp(elem->length, "hh"))
		return ((unsigned char)va_arg(ap, unsigned int));
	else
		return (va_arg(ap, ssize_t));
}

long			cast_len(va_list ap, t_printf *elem)
{
	if (!elem->length)
		return (va_arg(ap, int));
	else if (!ft_strcmp(elem->length, "l") || !ft_strcmp(elem->length, "ll") ||
				!ft_strcmp(elem->length, "j"))
		return (va_arg(ap, long));
	else if (!ft_strcmp(elem->length, "h"))
		return ((short)va_arg(ap, int));
	else if (!ft_strcmp(elem->length, "hh"))
		return ((char)va_arg(ap, int));
	else
		return (va_arg(ap, size_t));
}

int				treatbis(char c, t_printf *elem, va_list ap)
{
	if (c == 'o' || c == 'O' || c == 'u' || c == 'U' || c == 'x' ||
			c == 'X')
	{
		if (c == 'O' || c == 'U')
			return (ft_oux(va_arg(ap, unsigned long), c, elem));
		return (ft_oux(cast_lenu(ap, elem), c, elem));
	}
	else if (c == 'c' || c == 'C')
	{
		if (c == 'C' || (elem->length && !ft_strcmp(elem->length, "l")))
			return (ft_cwl(va_arg(ap, wint_t), elem));
		return (ft_c(va_arg(ap, int), elem));
	}
	else if (c == '%')
		return (ft_percent(elem));
	return (ft_invalid_input(c, elem));
}

int				treat(t_printf *elem, va_list ap)
{
	char c;

	c = elem->conversion;
	if (c == 's' || c == 'S')
	{
		if (c == 'S' || (elem->length && !ft_strcmp(elem->length, "l")))
			return (ft_smaj(va_arg(ap, wchar_t*), elem));
		return (ft_s(va_arg(ap, char*), elem));
	}
	else if (c == 'p')
		return (ft_p(va_arg(ap, void*), elem));
	else if (c == 'd' || c == 'i' || c == 'D')
	{
		if (c == 'D')
			return (ft_l(va_arg(ap, long), elem));
		return (ft_l(cast_len(ap, elem), elem));
	}
	else
		return (treatbis(c, elem, ap));
}
