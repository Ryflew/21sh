/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/18 22:35:32 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/08/10 03:06:35 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "tosh.h"

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

void	del_env(t_env *todel)
{
	free(todel->var_name);
	free(todel->var_content);
	free(todel->all);
	free(todel);
}

char	del_all_env(t_env **list)
{
	t_env	*tmp;

	if (!list)
		return (1);
	while (*list)
	{
		tmp = (*list)->next;
		del_env(*list);
		*list = tmp;
	}
	return (1);
}

char	**list_to_tabstr(t_list *list)
{
	t_list		*tmp;
	char		**args;
	uint32_t	size;
	uint32_t	i;

	if (!list)
		return (NULL);
	tmp = list;
	size = ft_list_size(list);
	if (!(args = (char**)malloc(sizeof(char*) * (size + 1))))
		ft_exiterror("ERROR: malloc failure", 0);
	i = 0;
	while (tmp)
	{
		args[i++] = ft_strdup((char*)tmp->data);
		tmp = tmp->next;
	}
	args[i] = NULL;
	return (args);
}

void	del_command_tree(t_tree *tree)
{
	if (tree->left)
		del_command_tree(tree->left);
	if (tree->right)
		del_command_tree(tree->right);
	if (tree->cmds)
		free(tree->cmds);
	if (tree->aggregations)
		ft_clear_list(&tree->aggregations, &free);
	if (tree->tmp_env)
		free(tree->tmp_env);
	free(tree);
	tree = NULL;
}
