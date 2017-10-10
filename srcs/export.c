/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/10 20:25:02 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/10/10 20:25:02 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

void		add_var_twoline(char *str1, char *str2, t_env **env)
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

char		export(char **av, t_env **export)
{
	char	*tmp;

	if (!*++av)
		print_env(*export);
	else
	{
		if ((tmp = ft_strchr(*av, '=')) && (tmp - *av) != 0)
		{
			add_var_oneline(*av, &(get_shell()->env));
			add_var_oneline(*av, &(get_shell()->export));
		}
		else if (!ft_strchr(*av, '='))
		{
			if ((tmp = find_env(get_shell()->shell_var, *av)) || \
				(tmp = find_env(*export, *av)))
			{
				add_var_twoline(*av, tmp, &(get_shell()->env));
				add_var_twoline(*av, tmp, &(get_shell()->export));
			}
			else
				add_var_twoline(*av, NULL, export);
		}
	}
	return (1);
}
