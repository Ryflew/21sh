#include "tosh.h"

void	add_var_to_shell(char *str1, char *str2, t_env **env)
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

char		export(char **av, t_env **export)
{
	char	*tmp;

	if (!*++av)
		print_env(*export);
	else
	{
		if (*(av + 1) && **(av + 1) == '-' && *(av + 2))
		{
			add_var_to_shell(*av, *(av + 2), &(get_shell()->env));
			add_var_to_shell(*av, *(av + 2), &(get_shell()->export));
		}
		else if (!*(av + 1))
		{
			if ((tmp = find_env(get_shell()->shell_var, *av)) || \
				(tmp = find_env(*export, *av)))
			{
				add_var_to_shell(*av, tmp, &(get_shell()->env));
				add_var_to_shell(*av, tmp, &(get_shell()->export));
			}
			else
				add_var_to_shell(*av, NULL, export);
		}
	}
	return (1);
}
