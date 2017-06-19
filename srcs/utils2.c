#include "21sh.h"

char	*remove_useless_space(char *str, int i, int j, int have_space)
{
	char	buff[ft_strlen(str)];
	char	c;

	c = 0;
	while (str[++i])
	{
		if (c)
			buff[++j] = str[i];
		else if (!ft_isblank(str[i]))
		{
			if (have_space && j != -1)
				buff[++j] = ' ';
			have_space = 0;
			buff[++j] = str[i];
		}
		else if (!c && ft_isblank(str[i]))
			have_space = 1;
		if (!c && (str[i] == '\'' || str[i] == '"' || str[i] == '`'))
			c = str[i];
		else if (c == str[i])
			c = '\0';
	}
	buff[++j] = '\0';
	return (ft_strdup(buff));
}

void	print_prompt(e_state state, char *op)
{
	if (state == BASIC_SHELL)
	{
		ft_putstr("[21sh:");
		ft_putstr(get_shell()->prompt);
		ft_putstr("] $> ");
	}
	else
	{
		ft_putchar('\n');
		if (op[ft_strlen(op) - 1] == '\'')
			ft_putstr("quote> ");
		else if (op[ft_strlen(op) - 1] == '"')
			ft_putstr("dquote> ");
		else if (op[ft_strlen(op) - 1] == '`')
			ft_putstr("bquote> ");
		else if (op[ft_strlen(op) - 1] == '(')
			ft_putstr("par> ");
		else if (op[ft_strlen(op) - 1] == '{')
			ft_putstr("aco> ");
		else if (op[ft_strlen(op) - 1] == '[')
			ft_putstr("cro> ");
		else if (op[ft_strlen(op) - 1] == '|')
			ft_putstr("pipe> ");
	}
	get_cursor(get_shell());
}
