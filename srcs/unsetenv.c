/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unsetenv.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdurst <bdurst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/11 17:44:52 by bdurst            #+#    #+#             */
/*   Updated: 2017/10/11 17:44:53 by bdurst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "tosh.h"

void	manage_unset(t_sh *shell, char **av)
{
	if (!*(av + 1))
	{
		errexit("unset", "Too few arguments.");
		return ;
	}
	if (find_env(shell->env, *(av + 1)))
		unset_env(av, &(shell->env));
	if (find_env(shell->export, *(av + 1)))
		unset_env(av, &(shell->export));
	if (find_env(shell->shell_var, *(av + 1)))
		unset_env(av, &(shell->shell_var));
}

void	unset_env(char **av, t_env **env)
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
		if (*env && !ft_strcmp(prev->var_name, (*env)->var_name))
			first = first->next;
		else if (*env)
			prev->next = (*env)->next;
		if (*env)
			del_env(*env);
		*env = first;
		av++;
	}
}
