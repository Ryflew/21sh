#include "tosh.h"

void	read_a(char *opt, char *var_name, t_sh *shell)
{
	char	*line;

	if (!var_name)
		var_name = ft_strdup("REPLY");
	else
		var_name = ft_strdup(var_name);
	reset_line_shell(shell);
	if (opt[6])
		line = read_line_echo(0, 0, 0);
	else if (opt[2])
		line = read_line_cano(shell, 1);
	else
	{
		line = get_line(shell, 0, &(shell->state), NULL);
		ft_putchar('\n');
	}
	if (!opt[5])
		remove_backslash(&line);
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
	reset_line_shell(shell);
	if (opt[6])
		line = read_line_echo(0, *delimiter, 0);
	else if (opt[2])
		line = read_line_cano(shell, 1);
	else
	{
		line = get_line(shell, 0, &(shell->state), NULL);
		ft_putchar('\n');
	}
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
	reset_line_shell(shell);
	if (opt[6])
		line = read_line_echo(0, 0, shell->read_nchar);
	else if (opt[2])
		line = read_line_cano(shell, 1);
	else
	{
		line = get_line(shell, 0, &(shell->state), NULL);
		ft_putchar('\n');
	}
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
