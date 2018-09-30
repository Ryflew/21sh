/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd4.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/30 19:32:20 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/09/30 19:32:39 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

void	cd_dot_dot(char **new_prompt, t_cd *opt)
{
	char	*tmp;
	char	*cwd;
	char	buff[4097];

	if (!(opt->is_logical && (cwd = find_env(get_shell()->env, "PWD"))))
		cwd = getcwd(buff, 4097);
	if (ft_strlen(cwd) > 1)
	{
		if ((ft_strrchr(cwd, '/') - cwd) == 0)
			tmp = ft_strdup("/");
		else
			tmp = ft_strsub(cwd, 0, ft_strrchr(cwd, '/') - cwd);
		ft_strdel(new_prompt);
		*new_prompt = ft_strdup(tmp);
		chdir(tmp);
		free(tmp);
	}
	else
	{
		ft_strdel(new_prompt);
		*new_prompt = ft_strdup(getcwd(buff, 4097));
	}
}
