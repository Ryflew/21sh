/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/29 17:18:28 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/09/29 17:29:05 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

void	rewrite_env(t_env *env, char *name, char *content)
{
	char	**av;

	if (!(av = (char**)malloc(sizeof(char*) * 4)))
		ft_exiterror("Malloc failed", 1);
	av[0] = ft_strdup("setenv");
	av[1] = ft_strdup(name);
	av[2] = ft_strdup(content);
	av[3] = NULL;
	set_env(av, &env);
	ft_strdelpp(&av);
}
