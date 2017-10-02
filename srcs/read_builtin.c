#include "tosh.h"

char	get_read_opt2(char *opt, char ***av, int i)
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
			else if (get_read_opt2(opt, av, i))
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

char	*read_line_echo(int fd, unsigned long deli, int nchar)
{
	char			buff[4096];
	int				i;
	unsigned long	c;

	deli = !deli ? ENTER : deli;
	ft_bzero(buff, 4096);
	i = -1;
	c = 0;
	while (read(fd, &c, sizeof(unsigned long)) != -1 && i < 4096)
	{
		if (c == deli || !(nchar - 1))
			return (ft_strdup(buff));
		else if (c >= 32 && c <= 126)
			buff[++i] = c;
		if (nchar > 0)
			--nchar;
		c = 0;
	}
	ft_putchar('\n');
	return (ft_strdup(buff));
}

char	*read_line_cano(t_sh *shell, int fd)
{
	char	*str;

	set_old_term(shell);
	get_next_line(fd, &str);
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
		line = read_line_echo(0, 0, 0);
	else if (opt[2])
		read_line_cano(shell, 1);
	else
	{
		line = get_line(shell, 0, &(shell->state), NULL);
		ft_putchar('\n');
	}
	if (!line)
		line = ft_strdup("");
	if (!opt[5])
		remove_backslash(&line);
	ft_putendl(line);
	read_addvar(shell, var_name, line);
	ft_strdel(&var_name);
	ft_strdel(&line);
}

void	read_d(char *opt, char *delimiter, t_sh *shell)
{
	char	*line;

	if (!delimiter || !*delimiter || !ft_isascii(*delimiter))
	{
		errexit("read", "DELIM character missing.");
		return ;
	}
	shell->read_delimiter = *delimiter;
	if (shell->op)
		free(shell->op);
	free(shell->total_command);
	shell->op = NULL;
	shell->total_command = NULL;
	shell->state = READ_CMD;
	line = NULL;
	if (opt[6])
		line = read_line_echo(0, *delimiter, 0);
	else if (opt[2])
		read_line_cano(shell, 1);
	else
	{
		line = get_line(shell, 0, &(shell->state), NULL);
		ft_putchar('\n');
	}
	if (!line)
		line = ft_strdup("");
	if (!opt[5])
		remove_backslash(&line);
	read_addvar(shell, "REPLY", line);
	ft_strdel(&line);
	shell->read_delimiter = 0;
}

void	read_n(char *opt, char *nb, t_sh *shell)
{
	char	*line;

	if (!nb || !*nb || !(shell->read_nchar = (int)ft_atoi(nb) + 1) || \
		shell->read_nchar <= 0 || shell->read_nchar > INT32_MAX)
	{
		errexit("read", "invalide number.");
		return ;
	}
	if (shell->op)
		free(shell->op);
	free(shell->total_command);
	shell->op = NULL;
	shell->total_command = NULL;
	shell->state = READ_CMD;
	line = NULL;
	if (opt[6])
		line = read_line_echo(0, 0, shell->read_nchar);
	else if (opt[2])
		read_line_cano(shell, 1);
	else
	{
		line = get_line(shell, 0, &(shell->state), NULL);
		ft_putchar('\n');
	}
	if (!line)
		line = ft_strdup("");
	if (!opt[5])
		remove_backslash(&line);
	read_addvar(shell, "REPLY", line);
	ft_strdel(&line);
	shell->read_nchar = 0;
}

void	read_p(char *opt, char *prompt, t_sh *shell)
{
	if (!prompt || !*prompt)
	{
		errexit("read", "invalide prompt.");
		return ;
	}
	ft_putstr(prompt);
	read_a(opt, NULL, shell);
}

void	read_u(char *opt, char *str_fd, t_sh *shell)
{
	int		fd;
	char	*line;

	if (!str_fd || !*str_fd || !(fd = ft_atoi(str_fd)))
	{
		errexit("read", "invalide file descriptor.");
		return ;
	}
	if (opt[6])
		line = read_line_echo(fd, 0, 0);
	else
		line = read_line_cano(shell, fd);
	if (!line)
		line = ft_strdup("");
	if (!opt[5])
		remove_backslash(&line);
	read_addvar(shell, "REPLY", line);
	ft_strdel(&line);
}

void	read_builtin(t_sh *shell, char **argv)
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