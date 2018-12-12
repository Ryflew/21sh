/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrealsplit.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/11 21:34:47 by bdurst2812        #+#    #+#             */
/*   Updated: 2018/12/12 18:15:19 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "string.h"
#include "io.h"

static size_t	ft_nb_words(char const *s, char c)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (s[i] && s)
	{
		if (s[i] == c)
		{
			j++;
			i++;
		}
		else
		{
			j++;
			while (s[i] && s[i] != c)
				i++;
		}
	}
	return (j);
}

char			**ft_strrealsplit(char const *s, char c)
{
	char	**tab;
	size_t	len;
	size_t	l_wrd;
	size_t	j;

	if (s && (tab = (char **)malloc(sizeof(char *) * (ft_nb_words(s, c) + 1))))
	{
		j = 0;
		len = 0;
		while (s[len])
		{
			l_wrd = 0;
			while (s[len + l_wrd] && s[len + l_wrd] != c)
				++l_wrd;
			tab[j++] = ft_strsub(s, len, s[len + l_wrd] == c ? l_wrd++ : l_wrd);
			len += l_wrd;
		}
		if (len > 0 && s[len - 1] == c)
			tab[j++] = ft_strsub(s, len, 1);
		tab[j] = NULL;
		return ((char **)tab);
	}
	return (NULL);
}
