/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdjoin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/14 02:24:06 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/03/14 02:26:58 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char	*ft_strdjoin(char *s1, char *s2, char *s3)
{
	char	*str;
	int		len;
	int		i;

	i = -1;
	len = ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3) + 1;
	if (!(str = (char*)malloc(sizeof(char) * len)))
		return (NULL);
	while (*s1)
	{
		str[++i] = *s1;
		s1++;
	}
	while (*s2)
	{
		str[++i] = *s2;
		s2++;
	}
	while (*s3)
	{
		str[++i] = *s3;
		s3++;
	}
	str[++i] = '\0';
	return (str);
}
