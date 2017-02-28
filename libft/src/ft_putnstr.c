/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/25 23:03:48 by vdarmaya          #+#    #+#             */
/*   Updated: 2016/12/14 14:38:49 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

void	ft_putnstr(char *str, int nbr)
{
	int		i;

	i = -1;
	while (str[++i] && (i < nbr))
		ft_putchar(str[i]);
}
