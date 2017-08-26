#include "tosh.h"

void		get_hash_opt(char ***hash, char *r_opt, char *p_opt, char *d_opt)
{
	char *tmp;

	while (**hash && ***hash == '-')
	{
		tmp = **hash;
		while (*(++tmp))
		{
			if (*tmp != 'r' && *tmp != 'p' && *tmp != 'd')
			{
				ft_putstr("hash: illegal option -- ");
				ft_putchar(***hash);
				ft_putchar('\n');
			}
			*r_opt = *tmp == 'r' ? 1 : *r_opt;
			*p_opt = *tmp == 'p' ? 1 : *p_opt;
			*d_opt = *tmp == 'd' ? 1 : *d_opt;
		}
		++(*hash);
	}
}

static void	del_hash_line(char *name, t_sh *shell)
{
	char	**out;

	if (find_env(shell->hash, name))
	{
		if (!(out = (char**)malloc(sizeof(char*) * 3)))
		{
			ft_putstr("Malloc failed.");
			exit(EXIT_FAILURE);
		}
		out[0] = ft_strdup("unsetenv");
		out[1] = ft_strdup(name);
		out[2] = NULL;
		unset_env(out, &(shell->hash), NULL);
		ft_strdelpp(&out);
	}
	else
		errexit("hash", "program name not found.");
}

void		add_hash_line(char *name, char *path, t_sh *shell)
{
	char	**new_av;
	int		i;

	if (!is_binary(path))
	{
		errexit("hash", "not valide path.");
		return ;
	}
	i = 0;
	if (!(new_av = (char**)malloc(sizeof(char*) * 4)))
		exit(EXIT_FAILURE);
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
	get_hash_opt(&av, &r_opt, &p_opt, &d_opt);
	if (d_opt && !*(av + 1))
		del_hash_line(*av, shell);
	else if (p_opt && *(av + 1) && !*(av + 2))
		add_hash_line(*av, *(av + 1), shell);
	else if (r_opt && shell->hash)
	{
		del_all_env(&shell->hash);
		shell->hash = NULL;
	}
}
