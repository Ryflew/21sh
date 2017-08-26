#include <stdlib.h>
#include "tosh.h"

void	unset_export(char **av, t_env **export)
{
	t_env	*first;
	t_env	*prev;

	first = *export;
	while (*av)
	{
		prev = *export;
		while (*export && ft_strcmp((*export)->var_name, *av))
		{
			prev = *export;
			*export = (*export)->next;
		}
		if (*export && !ft_strcmp(prev->var_name, (*export)->var_name))
			first = first->next;
		else if (*export)
			prev->next = (*export)->next;
		if (*export)
			del_env(*export);
		*export = first;
		av++;
	}
}

void	unset_env(char **av, t_env **env, t_env **export)
{
	t_env	*first;
	t_env	*prev;

	if (!*++av)
		errexit("unsetenv", "Too few arguments.");
	if (export)
		unset_export(av, export);
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
