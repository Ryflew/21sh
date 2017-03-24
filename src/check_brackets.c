/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_brackets.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/14 01:55:53 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/03/22 19:08:34 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "get_next_line.h"
#include "21sh.h"

static void	prompt_listen(char c, t_sh *shell)
{
	ft_putchar('\n');
	if (c == '\'')
		ft_putstr("quote> ");
	else if (c == '"')
		ft_putstr("dquote> ");
	else if (c == '`')
		ft_putstr("bquote> ");
	else if (c == '(')
		ft_putstr("par> ");
	else if (c == '{')
		ft_putstr("aco> ");
	else if (c == '[')
		ft_putstr("cro> ");
	else if (c == '|')
		ft_putstr("pipe> ");
	get_cursor(shell);
}

static void	clear_line(char **str, int j, char c)
{
	char	*tmp;

	if (j == -1 || (c != '\'' && c != '"' && c != '`'))
	{
		tmp = remove_useless_space(*str);
		free(*str);
		*str = tmp;
	}
}

static int	listen_std2(char **out, char *op, char **str, int *j)
{
	char	*tmp2;

	if (*j > -1 && op[*j] == '|' && !(op[(*j)--] = 0))
		return (-1);
	else if (*j > -1 && go_to_c(str, out, get_good_char(op[*j])))
	{
		if (--*j > -1 && !**str)
			return (-2);
		else if (*j > -1)
			return (-1);
		else if (!**str)
			return (0);
	}
	else
	{
		if ((*j == -1 || (op[*j] != '\'' && op[*j] != '"' && \
			op[*j] != '`')) && check_new_open(*str, op, j))
			return (1);
		tmp2 = *out;
		*out = ft_strdjoin(*out, *str, "\n");
		free(tmp2);
		return (-2);
	}
	return (0);
}

static char listen_std(int j, char **out, char *op, t_sh *shell)
{
	char	*str;
	int		value;
	char	*tofree;
	char	buff[3];

	prompt_listen(op[j], shell);
	while (1)
	{
		shell->is_listen_bracket = 1;
		str = get_line(shell, buff, 1);
		if (*str && str[0] == '\t')
		{
			*out = str;
			return (0);
		}
		clear_line(&str, j, op[j]);
		tofree = str;
		while (*str)
		{
			if ((value = listen_std2(out, op, &str, &j)) == -1)
				continue ;
			else if (value == -2)
				break ;
			else
			{
				free(tofree);
				return (value);
			}
		}
		free(tofree);
		prompt_listen(op[j], shell);
	}
	return (0);
}

char		check_quot_brackets(char *str, char **tmp, t_sh *shell)
{
	int		i;
	int		j;
	char	op[ft_strlen(str)];
	char	error;

	error = 0;
	i = -1;
	j = -1;
	while (str[++i])
	{
		if ((str[i] == '"' || str[i] == '\'' || str[i] == '`' || \
			str[i] == '|') && check_quot(str, op, &i, &j))
			break ;
		else
			if ((error = check_quot_brackets2(str, op, i, &j)))
				return (1);
	}
	if (j > -1)
	{
		*tmp = ft_strjoin(str, "\n");
		if (listen_std(j, tmp, op, shell))
			return (1);
	}
	else
		*tmp = ft_strdup(str);
	return (0);
}
