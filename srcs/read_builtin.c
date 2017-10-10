#include "tosh.h"

void		read_addvar(t_sh *shell, char *var_name, char *line)
{
	char	**av;

	av = (char**)malloc(sizeof(char*) * 4);
	av[0] = ft_strdup("setenv");
	av[1] = ft_strdup(var_name);
	av[2] = ft_strdup(line);
	av[3] = NULL;
	set_env(av, &(shell->shell_var));
	ft_strdelpp(&av);
}

void		remove_backslash(char **str)
{
	int		i;
	int		j;
	char	buff[ft_strlen(*str) * 3 + 1];

	buff[0] = 0;
	i = -1;
	j = -1;
	while ((*str)[++i])
	{
		if ((*str)[i] == '\\')
			continue ;
		else
			buff[++j] = (*str)[i];
	}
	free(*str);
	buff[++j] = 0;
	*str = ft_strdup(buff);
}

static char	get_read_opt2(char *opt, char ***av, int i)
{
	if (*(**av + i) == 'p')
		opt[4] = 1;
	else if (*(**av + i) == 'r')
		opt[5] = 1;
	else if (*(**av + i) == 's')
		opt[6] = 1;
	else if (*(**av + i) == 'u')
		opt[8] = 1;
	else
		return (1);
	return (0);
}

static void	get_read_opt(char *opt, char ***av)
{
	int		i;

	++(*av);
	while (**av && ***av == '-')
	{
		i = 1;
		while (*(**av + i))
		{
			if (*(**av + i) == 'a')
				opt[0] = 1;
			else if (*(**av + i) == 'd')
				opt[1] = 1;
			else if (*(**av + i) == 'e')
				opt[2] = 1;
			else if (*(**av + i) == 'n')
				opt[3] = 1;
			else if (get_read_opt2(opt, av, i))
				break ;
			++i;
		}
		++(*av);
	}
}

void		read_builtin(t_sh *shell, char **argv)
{
	char	opt[9];
	char	**av;

	av = argv;
	ft_bzero(opt, 9);
	get_read_opt(opt, &av);
	if (opt[0])
		read_a(opt, *av, shell);
	else if (opt[1])
		read_d(opt, *av, shell);
	else if (opt[3])
		read_n(opt, *av, shell);
	else if (opt[4])
		read_p(opt, *av, shell);
	else if (opt[8])
		read_u(opt, *av, shell);
	else
		read_a(opt, NULL, shell);
}
