/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/18 22:35:32 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/03/18 17:15:37 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "21sh.h"

t_env	*new_env(char *str)
{
	t_env	*elem;
	char	*tmp;

	if (!(elem = (t_env*)malloc(sizeof(t_env))))
		exit(EXIT_FAILURE);
	elem->all = ft_strdup(str);
	elem->var_name = ft_strsub(str, 0, ft_strchr(str, '=') - str);
	tmp = ft_strchr(str, '=') + 1;
	if (!*tmp)
		elem->var_content = ft_strdup("");
	else
		elem->var_content = ft_strdup(tmp);
	elem->next = NULL;
	return (elem);
}

t_tree	*init_tree()
{
	t_tree	*tree;

	if (!(tree = (t_tree*)malloc(sizeof(t_tree))))
		return (NULL);
	tree->cmd = NULL;
	tree->from_fd = -2;
	tree->to_fd = -2;
	tree->token = ENDOF;
	tree->left = NULL;
	tree->right = NULL;
	return (tree);
}

static void	free_one_tree(t_tree *tree)
{
	if (tree->cmd)
			ft_strdelpp(&(tree->cmd));
	if (tree->left)
	{
		free_one_tree(tree->left);
		free(tree->left);
	}
	if (tree->right)
	{
		free_one_tree(tree->right);
		free(tree->right);
	}
}

void	free_tree(t_tree **tree)
{
	int		i;

	i = -1;
	while (tree[++i])
	{
		free_one_tree(tree[i]);
		free(tree[i]);
	}
	free(tree);
}