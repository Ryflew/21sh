/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exclamation_point.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/10 20:24:48 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/12/03 13:26:47 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		if (i - nb >= 0 && nb <= i)
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

static int	history_excla(char *str, t_sh *shell)
{
	if (*str == '!')
		return (history_prompt(shell, 1, 1) - 1);
	else if (ft_isdigit(*str))
		return (history_prompt(shell, ft_atoi(str), 0));
	else if (*str == '-' && ft_isdigit(*(str + 1)))
		return (history_prompt(shell, ft_atoi(str + 1), 1));
	else
		return (history_string(shell, str));
}

char		check_history_excla(t_sh *shell, char **com, char *start, \
				char *tmp)
{
	char	*new;
	int		i;

	if (!(tmp = ft_strchr(*com, '!')) || (tmp - *com > 0 && *(tmp - 1) == '['))
		return (0);
	while ((tmp = ft_strchr(*com, '!')))
	{
		start = ft_strsub(*com, 0, tmp - *com);
		i = history_excla(++tmp, get_shell());
		if (shell->toaddstr)
		{
			new = ft_strstrjoin(start, shell->toaddstr, tmp + i);
			free(*com);
			*com = new;
			free(shell->toaddstr);
			shell->toaddstr = NULL;
		}
		else if (!ft_strchr(tmp, '!'))
			break ;
		ft_strdel(&start);
	}
	if (start)
		ft_strdel(&start);
	shell->toaddstr = *com;
	return (1);
}
