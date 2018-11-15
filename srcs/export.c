/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/10 20:25:02 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/11/15 17:16:00 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

static void	add_var_oneline(char *str, t_env **env)
{
	char	*tmp[4];
	char	*ptr;

	tmp[0] = ft_strdup("setenv");
	if ((ptr = ft_strchr(str, '=')))
	{
		tmp[1] = ft_strsub(str, 0, ptr - str);
		if (*(ptr + 1))
			tmp[2] = ft_strsub(str, ptr - str + 1, ft_strlen(str) - \
				ft_strlen(tmp[1]) - 1);
		else
			tmp[2] = ft_strdup("\'\'");
	}
	else
	{
		tmp[1] = ft_strdup(str);
		tmp[2] = ft_strdup("\'\'");
	}
	tmp[3] = NULL;
	set_env(tmp, env);
	ft_strdel(&tmp[0]);
	ft_strdel(&tmp[1]);
	ft_strdel(&tmp[2]);
}

static void	print_export(t_list *lst)
{
	char	*tmp;

	while (lst)
	{
		if ((tmp = find_env(get_shell()->shell_var, (char*)lst->data)))
		{
			ft_putstr("export ");
			ft_putstr((char*)lst->data);
			ft_putchar('=');
			ft_putendl(tmp);
		}
		else
		{
			ft_putstr("export ");
			ft_putendl((char*)lst->data);
		}
		NEXT(lst);
	}
}

static void	unset_var_env(char *var)
{
	char	*tab[3];

	tab[0] = "unsetenv";
	tab[1] = var;
	tab[2] = NULL;
	unset_env(tab, &get_shell()->env);
}

static void	remove_export(char **av, t_list **export)
{
	t_list *tmp;

	while (*++av)
	{
		if (!is_exported(*av, export))
			continue ;
		tmp = *export;
		while (tmp)
		{
			if (!ft_strcmp(*av, (char*)tmp->data))
			{
				if (!tmp->prev)
					*export = tmp->next;
				else
					tmp->prev->next = tmp->next;
				free(tmp->data);
				free(tmp);
				unset_var_env(*av);
				break ;
			}
			NEXT(tmp);
		}
	}
}

char		export(char **av, t_list **export)
{
	char	*tmp;

	if (!*++av || !ft_strcmp(*av, "-p"))
		print_export(*export);
	else if (!ft_strcmp(*av, "-n"))
		remove_export(av, export);
	else
	{
		if ((tmp = ft_strchr(*av, '=')) && (tmp - *av) != 0)
		{
			add_var_oneline(*av, &(get_shell()->shell_var));
			add_var_oneline(*av, &(get_shell()->env));
		}
		else if (!ft_strchr(*av, '=') && \
			(tmp = find_env(get_shell()->shell_var, *av)))
			add_var_twoline(*av, tmp, &(get_shell()->env));
		add_export(*av, &get_shell()->export);
	}
	return (1);
}
