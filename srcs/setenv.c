/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setenv.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/26 21:18:38 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/08/30 13:02:56 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "tosh.h"

static void	override_env(char **av, t_env **env)
{
	if (!ft_strcmp(*av, "PATH"))
	{
		del_all_env(&(get_shell()->hash));
		get_shell()->hash = NULL;
	}
	while (ft_strcmp((*env)->var_name, *av) != 0)
		*env = (*env)->next;
	free((*env)->var_content);
	free((*env)->all);
	if (!*++av)
		(*env)->var_content = ft_strdup("");
	else
		(*env)->var_content = ft_strdup(*av);
	(*env)->all = ft_strstrjoin((*env)->var_name, "=", (*env)->var_content);
}

static char	setenv_name(char **av)
{
	char	*tmp;

	tmp = *(av + 1);
	if (*tmp >= '0' && *tmp <= '9')
	{
		errexit("setenv", "Variable name must begin with a letter.");
		return (1);
	}
	return (0);
}

static char	setenv_size(char **av)
{
	int		i;

	i = 0;
	while (av[i] && i < 4)
		++i;
	if (i == 4)
	{
		errexit("setenv", "Too many arguments.");
		return (1);
	}
	if (*(av + 1))
		return (setenv_name(av));
	return (0);
}

static char	set_env_format_error(void)
{
	errexit("setenv", "Wrong format.\n\tType \"help setenv\" " \
		"for seeing usage.");
	return (1);
}

void		set_env(char **av, t_env **env)
{
	char	*tmp;
	t_env	*first;

	if (setenv_size(av))
		return ;
	first = *env;
	if (!*++av)
		print_env(*env);
	else if (ft_strchr(*av, '=') && set_env_format_error())
		return ;
	else if (!(find_env(*env, *av)))
	{
		while (*env && (*env)->next)
			*env = (*env)->next;
		tmp = get_tmp(av);
		if (!*env)
			*env = new_env(tmp);
		else
			(*env)->next = new_env(tmp);
		free(tmp);
	}
	else
		override_env(av, env);
	if (first)
		*env = first;
}
