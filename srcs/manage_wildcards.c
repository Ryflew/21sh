/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_wildcards.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/11 17:47:15 by bdurst            #+#    #+#             */
/*   Updated: 2018/11/29 15:01:18 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

static char	is_end_path(t_list *lexems)
{
	t_token	*token;

	while (lexems)
	{
		token = (t_token*)lexems->data;
		if (TYPE == END_EXPR)
			return (0);
		if (TYPE == EXPR && ft_strchr(VAL, '/'))
			return (1);
		lexems = lexems->next;
	}
	return (0);
}

static void	add_end_path(t_list *lexems, t_list **new_lexems)
{
	t_token	*token;
	char	copy;

	copy = 0;
	while (lexems)
	{
		token = (t_token*)lexems->data;
		if (TYPE == END_EXPR)
			break ;
		if (TYPE == EXPR && ft_strchr(VAL, '/'))
			++copy;
		if (copy)
		{
			if (TYPE == EXPR && ft_strchr(VAL, '/') && copy == 1)
				free_join(&((t_token*)(*new_lexems)->data)->value, \
				VAL + ft_strfind(VAL, 0, '/'));
			else
				ft_node_push_back(new_lexems, new_token(NULL, TYPE, VAL, BLK));
		}
		lexems = lexems->next;
	}
}

static char	fill_new_lexems(t_list *lexems, char end_path, char *join)
{
	t_list	*tmp;
	t_list	*new_lexems;

	new_lexems = NULL;
	ft_node_push_back(&new_lexems, new_token(NULL, SON, "", 0));
	ft_node_push_back(&new_lexems, new_token(NULL, EXPR, join, 1));
	if (end_path)
		add_end_path(lexems->next, &new_lexems->next);
	ft_node_push_back(&new_lexems, new_token(NULL, END_EXPR, "", 0));
	tmp = lexems;
	while (tmp)
	{
		if (((t_token*)(tmp->data))->type == END_EXPR && (!tmp->next || \
			((t_token*)(tmp->next->data))->type != SON))
			break ;
		tmp = tmp->next;
	}
	ft_add_list(tmp, new_lexems);
	return (1);
}

static void	init_glob(t_glob *glob, char *file_name)
{
	glob->dir = 0;
	glob->hidden_f_d = (file_name[0] == '.') ? 0 : 1;
}

void		manage_wildcards(t_list *lex, char *match, t_token *token)
{
	DIR				*dir;
	struct dirent	*ent;
	char			*dir_path;
	char			*join;
	t_glob			glob;

	dir_path = ft_strsub(VAL, 0, ft_strfind_by_end(VAL, '/'));
	if ((dir = open_dir(dir_path, token)))
	{
		while ((ent = readdir(dir)) != NULL)
		{
			init_glob(&glob, ent->d_name);
			join = (dir_path[0]) ? ft_strstrjoin(dir_path, "/", ent->d_name) \
			: ft_strdup(ent->d_name);
			if ((is_dir(join, token) || !is_end_path(lex->next)) && \
			((TYPE != EXPR && nmatch(ent->d_name, NULL, lex, glob)) || \
			(TYPE == EXPR && ((dir_path[0] && nmatch(ent->d_name, \
			VAL + ft_strfind_by_end(VAL, '/') + 1, lex, glob)) || \
			(!dir_path[0] && nmatch(ent->d_name, VAL, lex, glob))))))
				*match = fill_new_lexems(lex, is_end_path(lex->next), join);
			free(join);
		}
		closedir(dir);
	}
	free(dir_path);
}
