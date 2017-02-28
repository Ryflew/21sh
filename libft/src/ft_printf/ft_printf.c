/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/24 18:35:37 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/02/18 17:41:16 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <wchar.h>
#include <stdlib.h>
#include <unistd.h>
#include "../../include/libft.h"

int			del_elem(t_printf *elem, int nbr)
{
	if (elem->length)
		free(elem->length);
	free(elem);
	return (nbr);
}

t_printf	*creat_elem(void)
{
	t_printf	*elem;

	elem = (t_printf*)malloc(sizeof(t_printf));
	elem->flag_sharp = 0;
	elem->flag_zero = 0;
	elem->flag_minus = 0;
	elem->flag_plus = 0;
	elem->flag_space = 0;
	elem->width = 0;
	elem->precision = -1;
	elem->length = NULL;
	elem->conversion = 0;
	return (elem);
}

int			go_args(char **str, va_list ap)
{
	t_printf	*elem;
	char		c;

	elem = creat_elem();
	(*str)++;
	while (1)
	{
		if (*str == '\0')
			return (del_elem(elem, 0));
		if (!(check_flags(str, elem)))
			return (del_elem(elem, 0));
		if (!check_width(str, elem))
			return (del_elem(elem, 0));
		if (!check_precision(str, elem))
			return (del_elem(elem, 0));
		if (!check_len(str, elem))
			return (del_elem(elem, 0));
		if (!check_conv(str, &c, elem))
			return (del_elem(elem, 0));
		if ((c != '#') && (c != '0') && (c != '-') && (c != '+') &&
			(c != ' ') && !((c >= 48) && (c <= 57)) && (c != '.') &&
			(c != 'h') && (c != 'l') && (c != 'j') && (c != 'z'))
			break ;
	}
	return (del_elem(elem, treat(elem, ap)));
}

int			go_solve(char *str, va_list ap, int bytes)
{
	char		*tmp;
	int			count;

	if (*str == '\0')
		return (bytes);
	tmp = ft_strchr(str, '%');
	if (!tmp)
	{
		ft_putstr(str);
		return (bytes + ft_strlen(str));
	}
	else if (tmp > str)
	{
		ft_putnstr(str, tmp - str);
		return (go_solve(tmp, ap, bytes + (tmp - str)));
	}
	else
	{
		if ((count = go_args(&str, ap)) == -1)
			return (0);
		else
			return (go_solve(str, ap, bytes + count));
	}
}

int			ft_printf(const char *restrict format, ...)
{
	va_list		ap;
	int			count;

	if (!format)
		return (0);
	va_start(ap, format);
	count = go_solve((char*)format, ap, 0);
	va_end(ap);
	return (count);
}
