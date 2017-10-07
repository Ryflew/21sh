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
		tmp[2] = ft_strdup("\'\'");
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
		if (!ft_strchr(*av, '=') && !find_env(*export, *av) && (tmp = find_env(get_shell()->shell_var, *av)))
		{
			add_var_to_shell(*av, tmp, &(get_shell()->env));
			add_var_to_shell(*av, tmp, &(get_shell()->export));
		}
		else if (!ft_strchr(*av, '=') && (!*export || (*export && !find_env(*export, *av))))
			add_var_to_shell(*av, NULL, export);
		else
		{
			add_new_var(&av, &(get_shell()->env));
			add_new_var(&av, export);
		}
	}
	return (1);
}
