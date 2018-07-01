/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/10 20:24:44 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/07/01 22:22:06 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "tosh.h"

static void	env_usage(char invalid)
{
	ft_fputstr("env: illegal option -- ", 2);
	ft_fputchar(invalid, 2);
	ft_fputchar('\n', 2);
	ft_fputendl("usage: env [-i] [-u name] [name=value ...]", 2);
	ft_fputendl("           [utility [argument ...]]", 2);
}

static void	u_opt(char ***av, t_env **env)
{
	char	**out;

	if (!*(*av + 1))
		env_usage('u');
	else
	{
		if (!(out = (char**)malloc(sizeof(char*) * 3)))
			ft_exiterror("Malloc failed", 1);
		out[0] = ft_strdup("unsetenv");
		out[1] = ft_strdup(*++*av);
		out[2] = NULL;
		unset_env(out, env);
		ft_strdelpp(&out);
		++(*av);
	}
}

static void	env_command3(char **av, t_env **cpy)
{
	while (*av)
	{
		if (!ft_strcmp(*av, "env") && env_command(av, *cpy))
			break ;
		else if (ft_strchr(*av, '='))
		{
			add_new_var(&av, cpy);
			edit_env(cpy);
		}
		else
			edit_env(cpy);
		av++;
	}
}

static void	env_command2(char **av, t_env **cpy)
{
	if (!ft_strcmp(*av, "-i") && del_all_env(cpy))
	{
		*cpy = NULL;
		edit_env(cpy);
		++av;
	}
	else if (!ft_strcmp(*av, "-u"))
		u_opt(&av, cpy);
	else if (**av == '-' && *(*av + 1))
	{
		env_usage(*(*av + 1));
		return ;
	}
	env_command3(av, cpy);
}

char		env_command(char **av, t_env *env)
{
	t_env *cpy;

	if (!*++av)
		print_env(env);
	else
	{
		cpy = cpy_env(env);
		env_command2(av, &cpy);
		del_all_env(&cpy);
	}
	return (1);
}
