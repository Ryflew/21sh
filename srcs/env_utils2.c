/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/10 20:24:38 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/07/06 23:37:48 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

t_env	*cpy_env(t_env *env)
{
	t_env	*elem;
	t_env	*first;

	first = NULL;
	if (env)
	{
		elem = new_env(env->all);
		first = elem;
		env = env->next;
	}
	while (env)
	{
		elem->next = new_env(env->all);
		elem = elem->next;
		env = env->next;
	}
	return (first);
}

void	edit_env(t_env **env)
{
	if (!get_shell()->save_env)
		get_shell()->save_env = get_shell()->env;
	else
		del_all_env(&(get_shell()->env));
	get_shell()->env = cpy_env(*env);
}

void	add_new_var(char ***av, t_env **env)
{
	char	**new_av;

	if (**av[0] == '=')
	{
		errexit("env", "Invalid argument");
		return ;
	}
	if (!(new_av = (char**)malloc(sizeof(char*) * 4)))
		ft_exiterror("Malloc failed", 1);
	new_av[0] = ft_strdup("setenv");
	if (ft_strchr(**av, '='))
	{
		new_av[1] = ft_strsub(**av, 0, ft_strchr(**av, '=') - **av);
		new_av[2] = ft_strdup(ft_strchr(**av, '=') + 1);
	}
	else
	{
		new_av[1] = ft_strdup(**av);
		new_av[2] = ft_strdup("\'\'");
	}
	new_av[3] = NULL;
	set_env(new_av, env);
	ft_strdelpp(&new_av);
}

char	*get_tmp(char **av)
{
	char	*str;
	char	*tmp;

	if (!(*(av + 1)))
		return (ft_strjoin(*av, "="));
	else
	{
		str = ft_strdup(*(av + 1));
		tmp = ft_strstrjoin(*av, "=", str);
		free(str);
		return (tmp);
	}
}

void	inc_shlvl(t_env *env)
{
	char	*str[4];
	char	*tmp;
	int		nb;

	if (!(tmp = find_env(env, "SHLVL")))
		str[2] = ft_strdup("1");
	else
	{
		nb = ft_atoi(tmp);
		if (nb > 0)
		{
			tmp = ft_itoa(++nb);
			str[2] = ft_strdup(tmp);
		}
		else
			str[2] = ft_strdup("1");
	}
	str[0] = ft_strdup("setenv");
	str[1] = ft_strdup("SHLVL");
	str[3] = NULL;
	set_env(str, &env);
	free(str[0]);
	free(str[1]);
	free(str[2]);
}
