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

int		history_prompt(t_sh *shell, int nb, char is_minus)
{
	int		i;
	char	*tmp;

	i = get_history_size(shell);
	tmp = ft_itoa(nb);
	if (is_minus)
	{
		if (nb >= 0 && nb < i)
			shell->toaddstr = ft_strdup(shell->history[nb - 1]);
		else
			errexit("history", "Wrong number.");
		i = ft_strlen(tmp) + 1;
	}
	else
	{
		if (i - nb >= 0 && nb < i)
			shell->toaddstr = ft_strdup(shell->history[i - nb]);
		else
			errexit("history", "Wrong number.");
		i = ft_strlen(tmp);
	}
	free(tmp);
	return (i);
}

int	history_string(t_sh *shell, char *str)
{
	int	i;
	int	len;
	int	max;

	len = -1;
	while (str[++len] && str[len] != ' ')
		;
	max = get_history_size(shell);
	i = -1;
	while (++i < max)
	{
		if (!ft_strncmp(str, shell->history[i], len))
		{
			shell->toaddstr = ft_strdup(shell->history[i]);
			return (len);
		}
	}
	return (0);
}

int		history_excla(char *str, t_sh *shell)
{
	if (*str == '!')
		return (history_prompt(shell, 1, 1)); // !!
	else if (ft_isdigit(*str))
		return (history_prompt(shell, ft_atoi(str), 0)); // !n
	else if (*str == '-' && ft_isdigit(*(str + 1)))
		return (history_prompt(shell, ft_atoi(str + 1), 1)); // !-n
	else
		return (history_string(shell, str)); // cherche dans l'historique grace a la string
}

void		history_cmd(char **av, t_sh *shell)
{
	if (!*++av)
		print_all_history(shell, 0);
	// else if (**av == '!')
	// {
	// 	if (*(*av + 1) == '!' && !*(*av + 2))
	// 		history_prompt(shell, 1, 1); // !!
	// 	else if (string_is_numeric(*av + 1))
	// 		history_prompt(shell, ft_atoi(*av + 1), 0); // !n
	// 	else if (*(*av + 2) && *(*av + 1) == '-' && string_is_numeric(*av + 2))
	// 		history_prompt(shell, ft_atoi(*av + 2), 1); // !-n
	// 	else
	// 	{
	// 		if (*(av + 1))
	// 			errexit("history", "too many arguments.");
	// 		else
	// 			history_string(shell, *av); // cherche dans l'historique grace a la string
	// 	}
	// }
	else if (string_is_numeric(*av))
		print_all_history(shell, ft_atoi(*av));
	else
		errexit("history", "wrong pattern.");
}