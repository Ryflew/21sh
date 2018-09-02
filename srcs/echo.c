/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/10 20:24:16 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/09/01 16:35:02 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

static char	echo_env(char *str)
{
	if (*(str + 1) == '?')
		ft_putnbr(get_shell()->return_value);
	return (1);
}

static void	echo_octal(char **str)
{
	char	buff[4];
	int		i;
	int		c;

	++*str;
	ft_bzero(buff, 4);
	i = -1;
	while (++i < 3 && ft_isdigit((*str)[i]))
		buff[i] = (*str)[i];
	if (!i)
		return ;
	c = base_converter(ft_atoi(buff), 8, 10);
	if (ft_isascii(c))
	{
		ft_putchar((char)c);
		*str += i;
	}
}

static void	backslash(char **str)
{
	if (**str == 'a' && (++*str || !*str))
		ft_putchar('\a');
	else if (**str == 'b' && (++*str || !*str))
		ft_putchar('\b');
	else if (**str == 'f')
		ft_putchar('\f');
	else if (**str == 'n' && (++*str || !*str))
		ft_putchar('\n');
	else if (**str == 'r' && (++*str || !*str))
		ft_putchar('\r');
	else if (**str == 't' && (++*str || !*str))
		ft_putchar('\t');
	else if (**str == 'v' && (++*str || !*str))
		ft_putchar('\v');
	else if (**str == '\\')
	{
		ft_putchar('\\');
		++*str;
	}
	else if (**str == '0')
		echo_octal(str);
}

static char	print_echo(char *str, char *nflag, char last)
{
	while (*str)
	{
		if (*str == '$' && *(str + 1) && echo_env(str))
			break ;
		else if (*str == '\\' && *(str + 1) && (*(str + 1) == 'a' || \
			*(str + 1) == 'b' || *(str + 1) == 'c' || *(str + 1) == 'f' || \
			*(str + 1) == 'n' || *(str + 1) == 'r' || *(str + 1) == 't' || \
			*(str + 1) == 'v' || *(str + 1) == '\\' || *(str + 1) == '0'))
		{
			++str;
			backslash(&str);
			if (*str == 'c' && (*nflag = 1))
				return (1);
		}
		else
		{
			ft_putchar(*str);
			++str;
		}
	}
	if (last && !*nflag)
		ft_putchar('\n');
	return (0);
}

void		echo_builtin(char **av)
{
	char	nflag;

	if (*++av && **av == '-' && *(*av + 1) != 'n')
	{
		ft_fputstr("hash: illegal option -- ", 2);
		ft_fputchar(*(*av + 1), 2);
		ft_fputchar('\n', 2);
		get_shell()->return_value = 1;
		return ;
	}
	else if (*(*av + 1) == 'n' && (++av || !av))
		nflag = 1;
	else
		nflag = 0;
	if (!*av && !nflag)
		ft_putchar('\n');
	while (*av)
	{
		if (print_echo(*av, &nflag, *(av + 1) ? 0 : 1))
			return ;
		if (*(av + 1))
			ft_putchar(' ');
		++av;
	}
	get_shell()->return_value = 0;
}
