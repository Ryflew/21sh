/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_tab.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/10 20:26:05 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/09/01 17:00:50 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

static char	get_hash_opt(char ***hash, char *r_opt, char *p_opt, char *d_opt)
{
	char *tmp;

	while (**hash && ***hash == '-')
	{
		tmp = **hash;
		while (*(++tmp))
		{
			if (*tmp != 'r' && *tmp != 'p' && *tmp != 'd')
			{
				ft_fputstr("hash: illegal option -- ", 2);
				ft_fputchar(*tmp, 2);
				ft_fputchar('\n', 2);
				get_shell()->return_value = 1;
				get_shell()->have_write_error = 1;
				return (0);
			}
			*r_opt = *tmp == 'r' ? 1 : *r_opt;
			*p_opt = *tmp == 'p' ? 1 : *p_opt;
			*d_opt = *tmp == 'd' ? 1 : *d_opt;
		}
		++(*hash);
	}
	return (1);
}

static void	del_hash_line(char *name, t_sh *shell)
{
	char	**out;

	if (find_env(shell->hash, name))
	{
		if (!(out = (char**)malloc(sizeof(char*) * 3)))
			ft_exiterror("Malloc failed", 1);
		out[0] = ft_strdup("unsetenv");
		out[1] = ft_strdup(name);
		out[2] = NULL;
		unset_env(out, &(shell->hash));
		ft_strdelpp(&out);
	}
	else
		errexit("hash", "program name not found.");
}

void		add_hash_line(char *name, char *path, t_sh *shell, char mute)
{
	char	**new_av;
	int		i;

	if (!is_binary(path))
	{
		if (!mute)
			errexit("hash", "not a valide path.");
		return ;
	}
	i = 0;
	if (!(new_av = (char**)malloc(sizeof(char*) * 4)))
		ft_exiterror("Malloc failed", 1);
	new_av[0] = ft_strdup("setenv");
	new_av[1] = ft_strdup(name);
	new_av[2] = ft_strdup(path);
	new_av[3] = NULL;
	set_env(new_av, &(shell->hash));
	ft_strdelpp(&new_av);
}

char		is_in_hashtab(char *path, t_env *env)
{
	while (env)
	{
		if (!ft_strcmp(env->var_content, path))
			return (1);
		env = env->next;
	}
	return (0);
}

void		hash_tab(char **av, t_sh *shell)
{
	char	r_opt;
	char	p_opt;
	char	d_opt;

	if (!*(++av))
	{
		if (!shell->hash)
			errexit("hash", "hash table empty.");
		else
			print_env(shell->hash);
		return ;
	}
	r_opt = 0;
	p_opt = 0;
	d_opt = 0;
	if (!get_hash_opt(&av, &r_opt, &p_opt, &d_opt))
		return ;
	if (d_opt && !*(av + 1))
		del_hash_line(*av, shell);
	else if (p_opt && *(av + 1) && !*(av + 2))
		add_hash_line(*av, *(av + 1), shell, 0);
	else if (r_opt && shell->hash && del_all_env(&shell->hash))
		shell->hash = NULL;
}
