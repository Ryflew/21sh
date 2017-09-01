#include "tosh.h"

void		add_new_export(char *str, t_env **export)
{
	char	**new_av;
	int		i;

	i = 0;
	if (!(new_av = (char**)malloc(sizeof(char*) * 4)))
		exit(EXIT_FAILURE);
	new_av[0] = ft_strdup("setenv");
	new_av[1] = ft_strdup(str);
	new_av[2] = ft_strdup("\'\'");
	new_av[3] = NULL;
	set_env(new_av, export);
	ft_strdelpp(&new_av);
}

char		export(char **av, t_env **export)
{
	if (!*(av + 1))
		print_env(*export);
	else
	{
		if (!ft_strchr(*(av + 1), '='))
		{
			if (!*export || !find_env(*export, *(av + 1))) // le bordel ici
				add_new_export(*++av, export);
		}
		else
		{
			++av;
			add_new_var(&av, &(get_shell()->env));
			add_new_var(&av, export);
		}
	}
	return (1);
}
