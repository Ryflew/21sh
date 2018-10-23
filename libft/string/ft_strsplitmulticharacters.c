/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplitmulticharacters.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdurst <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/27 12:32:59 by bdurst            #+#    #+#             */
/*   Updated: 2016/05/27 12:32:59 by bdurst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "string.h"

static char		is_c_of_cs(char c, char *cs)
{
	size_t	i;

	i = -1;
	while (cs[++i])
		if (c == cs[i])
			return (1);
	return (0);
}

static size_t	ft_nb_words(char const *s, char *cs)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (s[i] && s)
	{
		if (is_c_of_cs(s[i], cs))
			i++;
		else
		{
			j++;
			while (s[i] && !is_c_of_cs(s[i], cs))
				i++;
		}
	}
	return (j);
}

char			**ft_strsplitmulticharacters(char const *s, char *cs)
{
	char	**tab;
	size_t	len;
	size_t	len_word;
	size_t	j;

	if (s && (tab = (char **)malloc(sizeof(char *) * (ft_nb_words(s, cs) + 1))))
	{
		j = 0;
		len = 0;
		while (s[len])
		{
			if (is_c_of_cs(s[len], cs))
				len++;
			len_word = len;
			while (s[len] && !is_c_of_cs(s[len], cs))
				len++;
			if (len > len_word)
				tab[j++] = ft_strsub(s, len_word, len - len_word);
		}
		tab[j] = NULL;
		return ((char **)tab);
	}
	return (NULL);
}
