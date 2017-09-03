#include <limits.h>
#include "tosh.h"

static int	get_history_size(t_sh *shell)
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
	while (str[++i])
		if (!ft_isdigit(str[i]))
			return (0);
	return (1);
}

void	history_prompt(t_sh *shell, int nb, char is_minus)
{
	int	i;

	i = get_history_size(shell);
	if (is_minus)
	{
		if (nb >= 0 && nb < i)
			ft_putstr(shell->history[nb]);
		else
			errexit("history", "Wrong number.");
	}
	else
	{
		if (i - nb >= 0 && nb < i)
			ft_putstr(shell->history[i - nb]);
		else
			errexit("history", "Wrong number.");
	}
}

void		history_cmd(char **av, t_sh *shell)
{
	if (!*++av)
		print_all_history(shell, 0);
	else if (**av == '!')
	{
		if (*(*av + 1) == '!' && !*(*av + 2))
			history_prompt(shell, 1, 1); // !!
		else if (string_is_numeric(*av + 1))
			history_prompt(shell, ft_atoi(*av + 1), 0); // !n
		else if (*(*av + 2) && *(*av + 1) == '-' && string_is_numeric(*av + 2))
			history_prompt(shell, ft_atoi(*av + 2), 1); // !-n
		else
		{
			if (*(av + 1))
				errexit("history", "too many arguments.");
			else
			{
				; // cherche dans l'historique grace a la string
			}
		}
	}
	else if (string_is_numeric(*av))
		print_all_history(shell, ft_atoi(*av));
	else
		errexit("history", "wrong pattern.");
}