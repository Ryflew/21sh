/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_tab2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/10 20:26:11 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/10/10 20:26:11 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include "tosh.h"

static void	validity_path(char *cmd, t_sh *shell, int i)
{
	DIR			*dir;
	t_dirent	*ent;
	char		*tmp;
	char		**tab;

	tab = ft_strsplit(find_env(shell->env, "PATH"), ':');
	while (tab[++i])
	{
		if (!(dir = opendir(tab[i])))
			continue ;
		while ((ent = readdir(dir)))
		{
			if (!ft_strcmp(ent->d_name, cmd))
			{
				tmp = ft_strstrjoin(tab[i], "/", ent->d_name);
				add_hash_line(cmd, tmp, get_shell());
				free(tmp);
				ft_strdelpp(&tab);
				closedir(dir);
				return ;
			}
		}
		closedir(dir);
	}
	ft_strdelpp(&tab);
}

void		try_add_hashtab(t_tree *node, t_sh *shell)
{
	if (node->cmds && is_builtins(node->cmds))
		validity_path(node->cmds[0], shell, -1);
}
