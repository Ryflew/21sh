/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd2.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/18 22:54:35 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/02/18 23:03:57 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "21sh.h"

char	*get_with_tilde(char *path, t_env *env)
{
	char	*home;
	char	*tmp;
	int		i;

	i = -1;
	if (!(home = find_env(env, "HOME")))
		return (ft_strdup(path));
	while (*home && path[++i] && (path[i] == *home))
		++home;
	if (*home)
		return (ft_strdup(path));
	if (!path[i + 1])
		return (ft_strdup("~"));
	home = ft_strsub(path + i + 1, 0, ft_strlen(path) - i - 1);
	tmp = ft_strjoin("~", home);
	free(home);
	return (tmp);
}
