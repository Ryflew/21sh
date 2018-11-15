/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unsetenv.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/11 17:44:52 by bdurst            #+#    #+#             */
/*   Updated: 2018/11/15 15:50:23 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "tosh.h"

static void	is_path_var(void)
{
	del_all_env(&(get_shell()->hash));
	get_shell()->hash = NULL;
}

void		manage_unset(t_sh *shell, char **av)
{
	if (!*(av + 1))
	{
		errexit("unset", "Too few arguments.");
		return ;
	}
	if (find_env(shell->env, *(av + 1)))
		unset_env(av, &(shell->env));
	if (find_env(shell->shell_var, *(av + 1)))
		unset_env(av, &(shell->shell_var));
}

static void	unset_env2(t_env **env, t_env **first, t_env *prev)
{
	if (*env && !ft_strcmp(prev->var_name, (*env)->var_name))
		*first = (*first)->next;
	else if (*env)
		prev->next = (*env)->next;
	if (!ft_strcmp((*env)->var_name, "PATH"))
		is_path_var();
	if (*env)
		del_env(*env);
}

void		unset_env(char **av, t_env **env)
{
	t_env	*first;
	t_env	*prev;

	if (!*++av)
		errexit("unsetenv", "Too few arguments.");
	first = *env;
	while (*av)
	{
		prev = *env;
		while (*env && ft_strcmp((*env)->var_name, *av))
		{
			prev = *env;
			*env = (*env)->next;
		}
		if (!*env)
		{
			*env = first;
			errexit("unsetenv", "Variable not found.");
		}
		else
			unset_env2(env, &first, prev);
		*env = first;
		av++;
	}
}
