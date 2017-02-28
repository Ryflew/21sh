/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trim_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/28 22:30:04 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/02/28 23:23:27 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static void	is_quot(char c, int *in_dquot, int *in_squot)
{
	if (str[i] == '"' && !*in_dquot)
		++(*in_dquot);
	else if (str[i] == '\'' && !*in_squot)
		++(*in_squot);
	if (str[i] == '"' && *in_dquot)
		--(*in_dquot);
	else if (str[i] == '\'' && *in_squot)
		--(*in_squot);
}

static int	count_sep(char *str)
{
	int		i;
	int		count;
	int		in_dquot;
	int		in_squot;

	i = -1;
	count = 1;
	in_dquot = 0;
	in_squot = 0;
	while (str[++i])
	{
		is_quot(str[i], &in_dquot, &in_squot)
		if (str[i] == ';' && !in_dquot && !in_squot)
			++count;
	}
	return (count);
}

static void	trim_input2(char ***out, char *cmd)
{
	int		j;
	int		nb;
	char	in_dquot;
	char	in_squot;

	in_dquot = 0;
	in_squot = 0;
	nb = -1;
	j = -1;
	while (cmd[++nb])
	{
		if (cmd[nb] != ';')
		{
			while (cmd[nb] && cmd[nb] != ';' || (cmd[nb] == ';' && (in_dquot || in_squot)))
			{
				is_quot(cmd[nb], &in_dquot, &in_squot);
				++nb;
			}
			out[++j] = ft_strsub(cmd, 0, nb);
			if (cmd[nb])
				++cmd;
			cmd += nb;
			nb = -1;
		}
		else
		{
			++cmd;
			nb = -1;
		}
	}
}

char	**trim_input(char *cmd)
{
	char	**out;
	int		nb;

	cmd = remove_useless_space(cmd); // a faire
	nb = count_sep(cmd);
	if (!(out = (char**)malloc(sizeof(char*) * (count_sep(cmd) + 1))))
	{
		ft_putstr("Malloc failed");
		return (NULL);
	}
	out[nb] = NULL;
	trim_input2(&out, cmd);
}

echo "test ; un deux"; echo "test"