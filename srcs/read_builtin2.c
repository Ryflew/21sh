#include "tosh.h"

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

void	reset_line_shell(t_sh *shell)
{
	if (shell->op)
		free(shell->op);
	free(shell->total_command);
	shell->op = NULL;
	shell->total_command = NULL;
	shell->state = READ_CMD;
}

