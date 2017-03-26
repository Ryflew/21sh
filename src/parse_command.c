/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/18 23:49:46 by vdarmaya          #+#    #+#             */
/*   Updated: 2017/03/25 23:01:12 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "21sh.h"

static e_token	get_token(char *str)
{
	if (*str == '>' && *(str + 1) && *(str + 1) == '>')
		return (DCHEVF);
	else if (*str == '<' && *(str + 1) && *(str + 1) == '<')
		return (DCHEVB);
	else if (*str == '>' && *(str + 1) && *(str + 1) != '&')
		return (CHEVF);
	else if (*str == '<' && *(str + 1) && *(str + 1) != '&')
		return (CHEVB);
	else if (*str == '&' && *(str + 1) && *(str + 1) == '&')
		return (AND);
	else if (*str == '|' && *(str + 1) && *(str + 1) == '|')
		return (OR);
	else if (*str == '|')
		return (PIPE);
	else
		return (NONE);
}

static void	find_token(char **command, char **tmp, t_tree **tree)
{
	int		i;

	i = 0;
	if (!*command)
		return ;
	while ((*command)[i])
	{
		if (((*tree)->token = get_token(*command + i)) != NONE)
		{
			*tmp = ft_strsub(*command, 0, i);
			if ((*tree)->token == DCHEVF || (*tree)->token == DCHEVB || \
				(*tree)->token == AND || (*tree)->token == OR)
				++i;
			if ((*tree)->token == DCHEVF && i > 0 && ft_isdigit(*command[i - 1]))
				// sortie sur descripteur de fichier
			*command += i;
			i = 0;
			return ;
		}
		else
			++i;
	}
	(*tree)->token = ENDOF;
	*tmp = ft_strsub(*command, 0, i);
	(*tmp)[i] = '\0';
	*command += i;
}

t_tree	*get_tree(char *command)
{
	t_tree	*tree;
	char	*tmp;

	if (!(tree = init_tree()))
		return (NULL);
	find_token(&command, &tmp, &tree);
	if (tree->token == ENDOF)
	{
		tree->cmd = split_quot_cmd(tmp);
		ft_puttabstr(tree->cmd);
		if (tmp)
			free(tmp);
		manage_redirection(&tree);
	}
	else
		get_tree_rec(&tree, tmp, command);
	return (tree);
}

t_tree	**get_command(char *command)
{
	char	**trimed;
	int		i;
	t_tree	**tree;

	if (!(trimed = trim_input(command)))
		return (NULL);
	if (!(tree = (t_tree**)malloc(sizeof(t_tree*) * ft_counttab(trimed) + 1)))
	{
		ft_putstr("Malloc failed !\n");
		return (NULL);
	}
	i = -1;
	while (trimed[++i])
		tree[i] = get_tree(trimed[i]);
	tree[i] = NULL;
	ft_strdelpp(&trimed);
	return (tree);
}
