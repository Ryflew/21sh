/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_cmd2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/10 20:27:21 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/10/10 20:27:22 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

static void	go_hist_opt(t_sh *shell, char **av, char *opt)
{
	if (opt[0])
	{
		ft_strdelpp(&(shell->history));
		shell->hist_first = 0;
	}
	else if (opt[1] && *av)
		hist_d(shell, ft_atoi(*av));
	else if (opt[2])
		hist_a(shell, *av);
	else if (opt[3])
		hist_r(shell, *av);
	else if (opt[4])
		hist_w(shell, *av);
	else if (opt[5])
		add_to_history(shell, *av);
}

static void	history_get_opt(char **av, char *opt, int i)
{
	++av;
	while (*av && **av == '-')
	{
		i = 1;
		while (**av)
		{
			if (*(*av + i) == 'c')
				opt[0] = 1;
			else if (*(*av + i) == 'd')
				opt[1] = 1;
			else if (*(*av + i) == 'a')
				opt[2] = 1;
			else if (*(*av + i) == 'r')
				opt[3] = 1;
			else if (*(*av + i) == 'w')
				opt[4] = 1;
			else if (*(*av + i) == 's')
				opt[5] = 1;
			else
				break ;
			++i;
		}
		++av;
	}
	go_hist_opt(get_shell(), av, opt);
}

void		add_to_history(t_sh *shell, char *command)
{
	int		i;
	char	*tmp;

	if (!command || !*command)
		return ;
	if (!ft_strcont(command, "\n"))
		add_line(shell, command);
	else
	{
		i = -1;
		while (command[++i] && command[i] != '\n')
			;
		tmp = ft_strsub(command, 0, i);
		add_line(shell, tmp);
		free(tmp);
	}
	shell->history_pos = -1;
}

void		treat_history_cmd(t_tree *tree)
{
	char	opt[6];

	if (tree->cmds && !ft_strcmp(tree->cmds[0], "history"))
	{
		ft_bzero(opt, 6);
		history_get_opt(tree->cmds, opt, 1);
	}
}
