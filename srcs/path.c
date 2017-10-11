/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/11 17:45:50 by bdurst            #+#    #+#             */
/*   Updated: 2017/10/11 19:19:05 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"
#include <sys/stat.h>
#include <dirent.h>

static char	*check_in_hashtab(t_tree *node, t_sh *shell)
{
	char	*content;

	if ((content = find_env(shell->hash, node->cmds[0])))
		return (ft_strdup(content));
	return (NULL);
}

static char	*get_absolute_path(char *content, t_tree *node)
{
	char	**tmp;
	int		i;
	char	v;

	tmp = ft_strsplit(content, ':');
	content = node->cmds[0];
	if (ft_strrchr(content, '/'))
		content = ft_strrchr(content, '/') + 1;
	i = -1;
	while (tmp[++i])
	{
		if ((v = check_path(content, tmp[i])) == 1)
		{
			content = ft_strstrjoin(tmp[i], "/", content);
			ft_strdelpp(&tmp);
			return (content);
		}
		else if (v == 2)
			break ;
	}
	ft_strdelpp(&tmp);
	return (NULL);
}

char		*get_path(t_tree *node, t_env *env, t_sh *shell)
{
	char	*content;

	content = NULL;
	if (node->cmds && (content = check_in_hashtab(node, shell)))
		return (content);
	if ((content = is_absolute(node, env, shell)))
		return (ft_strdup(content));
	if (*(node->cmds[0]) == '/')
		return (NULL);
	if (!(content = find_env(env, "PATH")))
		return (NULL);
	return (get_absolute_path(content, node));
}

static char	*is_absolute2(t_tree *node)
{
	t_stat	file;

	if (!lstat(*node->cmds, &file) && S_ISREG(file.st_mode) && \
		is_binary(*node->cmds) && !access(*node->cmds, R_OK | X_OK))
		return (ft_strdup(*node->cmds));
	else if (access(*node->cmds, F_OK) == -1)
	{
		errexit(*node->cmds, "No such file or directory.");
		return (NULL);
	}
	else if (access(*node->cmds, R_OK | X_OK) == -1)
	{
		errexit(*node->cmds, "Permission denied.");
		return (NULL);
	}
	return ((void*)1);
}

char		*is_absolute(t_tree *node, t_env *env, t_sh *shell)
{
	if (node->cmds && **node->cmds && **node->cmds == '/')
		return (is_absolute2(node));
	else if (node->cmds && **node->cmds && **node->cmds == '.' && \
		*(*node->cmds + 1) && *(*node->cmds + 1) == '/')
		return (current_binary(node, env, shell));
	return (NULL);
}
