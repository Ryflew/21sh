#include <limits.h>
#include "tosh.h"

int			get_history_size(t_sh *shell)
{
	int	i;

	i = -1;
	while (++i <= INT_MAX && shell->history[i])
		;
	return (i);
}

static void	print_all_history(t_sh *shell, int count)
{
	int	i;
	int	j;

	i = get_history_size(shell);
	if (!count || count > i)
		count = i;
	j = -1;
	while (--count >= 0)
	{
		ft_putstr("  ");
		ft_putnbr(++j + 1);
		ft_putchar(' ');
		ft_putendl(shell->history[--i]);
	}
}

static int	string_is_numeric(char *str)
{
	int	i;

	i = -1;
	if (!str)
		return (0);
	while (str[++i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
	}
	return (1);
}

void		history_cmd(char **av, t_sh *shell)
{
	if (!*++av)
		print_all_history(shell, 0);
	else if (string_is_numeric(*av))
		print_all_history(shell, ft_atoi(*av));
}