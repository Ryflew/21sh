/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_useless_space.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/28 23:21:57 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/03/01 18:12:08 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*remove_useless_space(char *str)
{
	int		i;
	int		j;
	int		have_space;
	char	*out;
	char	buff[ft_strlen(str)];

	ft_bzero(buff, ft_strlen(str));
	have_space = 0;
	j = -1;
	i = -1;
	while (str[++i])
	{
		if (str[i] != ' ' && str[i] != '\t')
		{
			if (have_space && j != -1)
			{
				buff[++j] = ' ';
				have_space = 0;
			}
			buff[++j] = str[i];
		}
		else
			have_space = 1;
	}
	return (ft_strdup(buff));
}
