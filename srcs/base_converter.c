/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base_converter.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/08 01:13:00 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/05/08 01:13:50 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	base_converter(int nb, int frombase, int tobase)
{
	int result = 0;
	int i = 1;

	while (nb != 0)
	{
		result += (nb % tobase) * i;
		nb /= tobase;
		i *= frombase;
	}
	return (result);
}
