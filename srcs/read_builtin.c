#include "tosh.h"

void	get_read_opt(char *opt, char ***av)
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
			else if (*(**av + i) == 'p')
				opt[4] = 1;
			else if (*(**av + i) == 'r')
				opt[5] = 1;
			else if (*(**av + i) == 's')
				opt[6] = 1;
			else if (*(**av + i) == 't')
				opt[7] = 1;
			else if (*(**av + i) == 'u')
				opt[8] = 1;
			else
				break ;
			++i;
		}
		++(*av);
	}
}

void	read_addvar(t_sh *shell, char *var_name, char *line)
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

void	remove_backslash(char **str)
{
	int 	i;
	int		j;
	char	buff[ft_strlen(*str)];

	ft_bzero(buff, ft_strlen(*str));
	i = -1;
	j = -1;
	while ((*str)[++i])
	{
		if ((*str)[i] == '\\')
			continue ;
		else
			buff[++j] = (*str)[i];
	}
	if (!*buff)
		return ;
	free(*str);
	*str = ft_strdup(buff);
}

char	*read_line_echo(unsigned long deli)
{
	char			buff[4096];
	int				i;
	unsigned long	c;

	deli = !deli ? ENTER : deli;
	ft_bzero(buff, 4096);
	i = -1;
	while (read(0, &c, sizeof(unsigned long)) != -1 && i < 4096)
	{
		if (c == deli)
			return (ft_strdup(buff));
		else if (c >= 32 && c <= 126)
			buff[++i] = c;
	}
	ft_putchar('\n');
	return (ft_strdup(buff));
}

char	*read_line_cano(t_sh *shell)
{
	char	*str;

	set_old_term(shell);
	get_next_line(1, &str);
	set_our_term(shell);
	return (str);
}

void	read_a(char *opt, char *var_name, t_sh *shell)
{
	char	*line;

	line = NULL;
	if (!var_name)
		var_name = ft_strdup("REPLY");
	else
		var_name = ft_strdup(var_name);
	if (shell->op)
		free(shell->op);
	free(shell->total_command);
	shell->op = NULL;
	shell->total_command = NULL;
	shell->state = READ_CMD;
	if (opt[6])
		line = read_line_echo(0);
	else if (opt[2])
		read_line_cano(shell);
	else
	{
		line = get_line(shell, 0, &(shell->state), NULL);
		ft_putchar('\n');
	}
	if (!line)
		line = ft_strdup("");
	remove_backslash(&line);
	read_addvar(shell, var_name, line);
	ft_strdel(&var_name);
	ft_strdel(&line);
}

void	read_d(char *opt, char *delimiter, t_sh *shell)
{
	char	*var_name;
	char	*line;

	if (!delimiter || !*delimiter || !ft_isascii(*delimiter))
	{
		errexit("read", "DELIM character missing.");
		return ;
	}
	shell->read_delimiter = *delimiter;
	var_name = ft_strdup("REPLY");
	if (shell->op)
		free(shell->op);
	free(shell->total_command);
	shell->op = NULL;
	shell->total_command = NULL;
	shell->state = READ_CMD;
	line = NULL;
	if (opt[6])
		line = read_line_echo(*delimiter);
	else if (opt[2])
		read_line_cano(shell);
	else
	{
		line = get_line(shell, 0, &(shell->state), NULL);
		ft_putchar('\n');
	}
	if (!line)
		line = ft_strdup("");
	remove_backslash(&line);
	read_addvar(shell, var_name, line);
	ft_strdel(&var_name);
	ft_strdel(&line);
	shell->read_delimiter = 0;
}

void	read_builtin(t_sh *shell, char **argv)
{
	char	opt[9];
	char	**av;

	av = argv;
	ft_bzero(opt, 9);
	get_read_opt(opt, &av);
	if (opt[0]) // fait !
		read_a(opt, *av, shell);
	else if (opt[1])
		read_d(opt, *av, shell);
	else if (opt[3])
		;//read_n();
	else if (opt[4])
		;//read_p();
	else if (opt[7])
		;//read_t();
	else if (opt[8])
		;//read_u();
	else
		read_a(opt, NULL, shell);
}