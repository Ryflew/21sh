/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/18 22:20:57 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/07/06 23:44:06 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "tosh.h"

t_env	*get_env(char **env)
{
	t_env	*first;
	t_env	*elem;

	if (!*env)
		return (NULL);
	if (*env)
	{
		first = new_env(*env);
		elem = first;
	}
	while (++env && *env)
	{
		if (ft_strlen(*env) > MAX_CMD)
		{
			write(2, *env, ft_strchr(*env, '=') - *env);
			ft_fputstr(": Environment variable too large\n", 2);
			continue ;
		}
		elem->next = new_env(*env);
		elem = elem->next;
	}
	return (first);
}

void	print_env(t_env *env)
{
	while (env)
	{
		ft_putendl(env->all);
		env = env->next;
	}
}

char	*find_env(t_env *env, char *name)
{
	while (env)
	{
		if (!ft_strcmp(env->var_name, name))
			return (env->var_content);
		env = env->next;
	}
	return (NULL);
}

char	**conv_env(t_env *env)
{
	t_env	*tmp;
	char	**out;
	int		i;

	i = 0;
	tmp = env;
	while (tmp && ++i)
		tmp = tmp->next;
	if (i == 0)
		return (NULL);
	if ((out = (char**)malloc(sizeof(char*) * (i + 1))) == NULL)
		ft_exiterror("Malloc failed.", 1);
	out[i] = NULL;
	i = -1;
	while (env)
	{
		out[++i] = ft_strdup(env->all);
		env = env->next;
	}
	return (out);
}

char	check_alpha(char *str)
{
	while (*str)
	{
		if (!ft_isalpha(*str))
		{
			errexit("setenv", "Variable name must contain alphanumeric " \
					"characters.");
			return (1);
		}
		++str;
	}
	return (0);
}
