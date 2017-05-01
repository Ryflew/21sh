/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unicode.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/29 18:36:25 by vdarmaya          #+#    #+#             */
/*   Updated: 2016/12/14 14:41:57 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/libft.h"

void	one_byte(char c)
{
	char	nbr;

	nbr = 2;
	nbr = (nbr << 6);
	nbr += c;
	ft_putchar(nbr);
}

void	four_bytes(wchar_t c)
{
	char	nbr;

	nbr = 30;
	nbr = (nbr << 3);
	nbr += (char)(c >> 18);
	c = (c & 65535);
	ft_putchar(nbr);
	nbr = 2;
	nbr = (nbr << 6);
	nbr += (char)(c >> 12);
	c = (c & 4095);
	ft_putchar(nbr);
	nbr = 2;
	nbr = (nbr << 6);
	nbr += (char)(c >> 6);
	c = (c & 63);
	ft_putchar(nbr);
	one_byte(c);
}

void	three_bytes(wchar_t c)
{
	int	nbr;

	nbr = 14;
	nbr = (nbr << 4);
	nbr += (char)(c >> 12);
	c = (c & 4095);
	ft_putchar(nbr);
	nbr = 2;
	nbr = (nbr << 6);
	nbr += (char)(c >> 6);
	c = (c & 63);
	ft_putchar(nbr);
	one_byte(c);
}

void	two_bytes(wchar_t c)
{
	int		nbr;

	nbr = 6;
	nbr = (nbr << 5);
	nbr += (c >> 6);
	c = (c & 63);
	ft_putchar(nbr);
	one_byte(c);
}
