/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/18 22:35:32 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/07/10 12:24:49 by vdarmaya         ###   ########.fr       */
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
		ft_exiterror("ERROR: malloc failure\n", 0);
	args[size] = NULL;
	i = 0;
	while (tmp)
	{
		args[i++] = ft_strdup((char*)tmp->data);
		tmp = tmp->next;
	}
	return (args);
}

void	del_command_tree(t_tree *tree)
{
	if (tree->left)
		del_command_tree(tree->left);
	else if (tree->right)
		del_command_tree(tree->right);
	if (tree->cmds)
		ft_strdelpp(&(tree->cmds));
	if (tree->token && tree->token->value)
	{
		free(tree->token->value);
		free(tree->token);
	}
	if (tree->tmp_env)
		ft_strdelpp(&(tree->tmp_env));
	free(tree);
}