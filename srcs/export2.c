/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/15 17:09:26 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/11/15 17:16:23 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

t_list	*add_to_export(char **env)
{
	t_list	*first;
	t_list	*elem;
	char	*tmp;

	if (!*env)
		return (NULL);
	tmp = ft_strsub(*env, 0, ft_strchr(*env, '=') - *env);
	first = ft_create_node((void*)tmp);
	elem = first;
	while (++env && *env)
	{
		if (ft_strlen(*env) > MAX_CMD)
		{
			write(2, *env, ft_strchr(*env, '=') - *env);
			ft_fputstr(": Environment variable too large\n", 2);
			continue ;
		}
		tmp = ft_strsub(*env, 0, ft_strchr(*env, '=') - *env);
		elem->next = ft_create_node(((void*)tmp));
		elem->next->prev = elem;
		elem = elem->next;
	}
	return (first);
}

char	is_exported(char *name, t_list **export)
{
	t_list *tmp;

	tmp = *export;
	while (tmp)
	{
		if (!ft_strcmp((char*)tmp->data, name))
			return (1);
		NEXT(tmp);
	}
	return (0);
}

void	add_var_twoline(char *str1, char *str2, t_env **env)
{
	char	**tmp;

	if (!(tmp = (char**)malloc(sizeof(char*) * 4)))
		ft_exiterror("Malloc failed", 1);
	tmp[0] = ft_strdup("setenv");
	tmp[1] = ft_strdup(str1);
	if (str2 && *str2)
		tmp[2] = ft_strdup(str2);
	else
		tmp[2] = ft_strdup("");
	tmp[3] = NULL;
	set_env(tmp, env);
	ft_strdelpp(&tmp);
}

void	add_export(char *name, t_list **export)
{
	t_list	*tmp;
	char	*tmp2;

	name = \
		(tmp2 = ft_strchr(name, '=')) ? \
		ft_strsub(name, 0, tmp2 - name) : ft_strdup(name);
	if (!*export && (*export = ft_create_node((void*)name)))
		return ;
	if (is_exported(name, export))
	{
		free(name);
		return ;
	}
	tmp = *export;
	while (tmp)
	{
		if (!tmp->next)
		{
			tmp->next = ft_create_node((void*)name);
			tmp->next->prev = tmp;
			break ;
		}
		NEXT(tmp);
	}
	return ;
}
