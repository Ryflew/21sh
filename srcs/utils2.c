#include "tosh.h"

static void	basic_prompt(void)
{
	ft_putchar('[');
	ft_putstr(C_ITA);
	ft_putstr(C_CYAN);
	ft_putstr("21sh");
	ft_putstr(C_RESET);
	ft_putchar(':');
	ft_putstr(C_GREEN);
	ft_putstr(get_shell()->prompt);
	ft_putstr(C_RESET);
	ft_putstr("] $> ");
}

void		print_prompt(e_state state, char *op)
{
	if (state == BASIC_SHELL)
		basic_prompt();
	else if (op)
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
		else if (op[ft_strlen(op) - 1] == '|')
			ft_putstr("pipe> ");
	}
	get_cursor(get_shell());
}
