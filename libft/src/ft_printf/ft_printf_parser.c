/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_parser.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/25 22:51:33 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/02/18 17:41:14 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h>
#include <stdlib.h>
#include "../../include/libft.h"

char	*check_flags(char **str, t_printf *elem)
{
	if (**str == '#' || **str == ' ' || **str == '+' || **str == '-' ||
		**str == '0')
	{
		if (**str == '#')
			elem->flag_sharp = 1;
		else if (**str == '0')
			elem->flag_zero = 1;
		else if (**str == '-')
			elem->flag_minus = 1;
		else if (**str == '+')
			elem->flag_plus = 1;
		else if (**str == ' ')
			elem->flag_space = 1;
		(*str)++;
		return (check_flags(str, elem));
	}
	else
		return (*str);
}

char	*check_width(char **str, t_printf *elem)
{
	char	*tmp;
	char	*tmp2;
	int		i;

	if (!str || !elem)
		return (NULL);
	if ((**str < 48) || (**str > 57))
		return (*str);
	tmp2 = *str;
	while ((*tmp2 >= 48) && (*tmp2 <= 57))
		tmp2++;
	tmp = (char*)malloc(sizeof(char) * ((tmp2 - *str) + 1));
	i = -1;
	while ((**str >= 48) && (**str <= 57))
	{
		tmp[++i] = **str;
		(*str)++;
	}
	tmp[++i] = '\0';
	elem->width = ft_atoi(tmp);
	free(tmp);
	return (*str);
}

char	*check_precision(char **str, t_printf *elem)
{
	char	*tmp;
	char	*tmp2;
	int		i;

	if (**str != '.')
		return (*str);
	(*str)++;
	elem->precision = 0;
	if ((**str < 48) || (**str > 57))
		return (*str);
	tmp2 = *str;
	while ((*tmp2 >= 48) && (*tmp2 <= 57))
		tmp2++;
	tmp = (char*)malloc(sizeof(char) * ((tmp2 - *str) + 1));
	i = -1;
	while ((**str >= 48) && (**str <= 57))
	{
		tmp[++i] = **str;
		(*str)++;
	}
	tmp[++i] = '\0';
	elem->precision = ft_atoi(tmp);
	free(tmp);
	return (*str);
}

char	*check_len(char **s, t_printf *elem)
{
	char	c;

	c = **s;
	if (elem->length && (elem->length[0] > c))
		(*s)++;
	if (elem->length && (elem->length[0] > c))
		c = 0;
	else if (elem->length && (elem->length[0] < c) && (c == 'h' || c == 'l' ||
			c == 'j' || c == 'z'))
		free(elem->length);
	if (c && (c == 'h' || c == 'l' || c == 'j' || c == 'z'))
	{
		if ((c == 'h' && *(*s + 1) == 'h') || (c == 'l' && *(*s + 1) == 'l'))
			elem->length = ft_strnew(3);
		else
			elem->length = ft_strnew(2);
		elem->length[0] = c;
		if ((c == 'h' && *(*s + 1) == 'h') || (c == 'l' && *(*s + 1) == 'l'))
			elem->length[1] = (c == 'h') ? 'h' : 'l';
		if (elem->length[1] != '\0')
			(*s)++;
		(*s)++;
	}
	return (*s);
}

int		check_conv(char **str, char *cha, t_printf *elem)
{
	char	c;

	c = **str;
	if ((c == 's') || (c == 'S') || (c == 'p') || (c == 'd') || (c == 'D') ||
		(c == 'i') || (c == 'o') || (c == 'O') || (c == 'u') || (c == 'U') ||
		(c == 'x') || (c == 'X') || (c == 'c') || (c == 'C') || (c == '%') || c)
	{
		elem->conversion = **str;
		*cha = **str;
		if ((c != '#') && (c != '0') && (c != '-') && (c != '+') &&
			(c != ' ') && !((c >= '0') && (c <= '9')) && (c != '.') &&
			(c != 'h') && (c != 'l') && (c != 'j') && (c != 'z'))
			(*str)++;
		return (1);
	}
	return (0);
}
