#include "tosh.h"

static int	history_prompt(t_sh *shell, int nb, char is_minus)
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

static int	history_string(t_sh *shell, char *str)
{
	int	i;
	int	len;
	int	max;

	if (!str || !*str)
		return (0);
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

int			history_excla(char *str, t_sh *shell)
{
	if (*str == '!')
		return (history_prompt(shell, 1, 1));
	else if (ft_isdigit(*str))
		return (history_prompt(shell, ft_atoi(str), 0));
	else if (*str == '-' && ft_isdigit(*(str + 1)))
		return (history_prompt(shell, ft_atoi(str + 1), 1));
	else
		return (history_string(shell, str));
}
