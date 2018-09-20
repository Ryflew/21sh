/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   completion3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/10 20:23:49 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/09/20 13:06:30 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

char	*find_match_binary(t_sh *shell, char *tosearch)
{
	int		i;
	char	*content;
	char	*out;
	char	**tmp;

	if (!(content = find_env(shell->env, "PATH")))
		return (NULL);
	tmp = ft_strsplit(content, ':');
	content = tosearch;
	i = -1;
	while (tmp[++i])
		if ((out = check_dir_content(content, tmp[i])))
		{
			ft_strdelpp(&tmp);
			return (out);
		}
	ft_strdelpp(&tmp);
	return (NULL);
}

void	current_completion(char **str)
{
	char	*cwd;
	char	*tmp;
	char	buff[4097];

	if ((cwd = getcwd(buff, 4097)))
	{
		tmp = *str;
		*str = ft_strjoin(cwd, *str + 1);
		free(tmp);
	}
	else
	{
		free(*str);
		*str = NULL;
	}
}
