/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dir.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/10 20:24:12 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/12/09 17:01:37 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"
#include <sys/stat.h>

DIR			*open_dir(char *dir_name, t_token *token)
{
	DIR				*dir;
	char			*dir_path;

	if (TYPE != EXPR || VAL[0] != '/')
		dir_path = ft_strjoin("./", dir_name);
	else
		dir_path = dir_name;
	if (!(dir = opendir(dir_path)))
	{
		if (dir_path[0] == '.')
			free(dir_path);
		ft_putstr("ERROR: opendir failure -> ");
		ft_putendl(dir_name);
		return (NULL);
	}
	if (dir_path[0] == '.')
		free(dir_path);
	return (dir);
}

int			is_dir(char *dir_name, t_token *token)
{
	struct stat	statbuf;
	char		*dir_path;

	if (TYPE != EXPR || VAL[0] != '/')
		dir_path = ft_strjoin("./", dir_name);
	else
		dir_path = dir_name;
	if (stat(dir_path, &statbuf) != 0)
	{
		if (dir_path[0] == '.')
			free(dir_path);
		return (0);
	}
	if (dir_path[0] == '.')
		free(dir_path);
	return (S_ISDIR(statbuf.st_mode));
}

static char	clear_old_expr2(t_list **lexems, char match)
{
	t_token *token;

	token = (t_token*)(*lexems)->data;
	if (TYPE == END_EXPR)
	{
		if (match)
			ft_pop_node(lexems, (void*)&clear_lexems);
		else
			*lexems = (*lexems)->next;
		return (1);
	}
	if (match)
		ft_pop_node(lexems, (void*)&clear_lexems);
	else
		*lexems = (*lexems)->next;
	return (0);
}

void		clear_old_expr(t_list **lexems, t_list **first_lexems, char match)
{
	t_token *token;
	t_list	*save;

	while (*lexems)
	{
		token = (t_token*)(*lexems)->data;
		if (!is_glob_token(TYPE))
			break ;
		if (match)
			ft_pop_node(lexems, (void*)&clear_lexems);
		save = *lexems;
		*lexems = (*lexems)->prev;
	}
	*lexems = save;
	while (*lexems)
	{
		if (clear_old_expr2(lexems, match))
			break ;
	}
	if (match && (!*lexems || !(*lexems)->prev))
		*first_lexems = *lexems;
}
