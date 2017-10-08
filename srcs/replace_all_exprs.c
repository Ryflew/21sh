#include "tosh.h"

static char		check_son_expr(t_list **lexems, t_list **save_son, t_list **save,
				char *son)
{
	e_token	type;
	
	type = ((t_token*)((*lexems)->data))->type;
	if (type == SON)
	{
		*save_son = *lexems;
		*son = 1;
	}
	else
	{
		if (!*save && (is_glob_token(type) || type == EXPR))
			*save = *lexems;
		if (type == S_WILDCARD || type == Q_WILDCARD || type == E_WILDCARD
			|| type == LBKT || type == RBKT || type == LBRC)
		{
			*lexems = *save;
			if (*save_son)
				ft_pop_node(save_son, NULL);
			return (1);
		}
	}
	return (0);
}

static char		search_expr(t_list **lexems)
{
	e_token	type;
	t_list	*save;
	t_list	*save_son;
	char	son;

	save = NULL;
	save_son = NULL;
	son = 0;
	while (*lexems)
	{
		type = ((t_token*)((*lexems)->data))->type;
		if (type == END_EXPR)
		{
			save = NULL;
			save_son = NULL;
			son = 0;
		}
		else if (check_son_expr(lexems, &save, &save_son, &son))
			return (son);
		*lexems = (*lexems)->next;
	}
	return (0);
}

static t_list	*no_son_expr(t_list **first_lexems, t_list *save)
{
	t_list	*tmp;

	tmp = save;
	clear_old_expr(&tmp, first_lexems, 0);
	if (tmp && ((t_token*)(tmp->data))->type == SON)
		clear_old_expr(&save, first_lexems, 1);
	else
		merge_expr_to_word(save);
	return (NULL);
}

void			replace_all_exprs(t_list **first_lexems)
{
	t_list	*lexems;
	char	match;
	char	son;
	t_list	*save;

	lexems = *first_lexems;
	son = search_expr(&lexems);
	while (lexems)
	{
		if (!son)
			save = lexems;
		match = 0;
		manage_wildcards(lexems, &match);
		if (son)
			clear_old_expr(&lexems, first_lexems, 1);
		else
			clear_old_expr(&lexems, first_lexems, 0);
		if (!(son = search_expr(&lexems)))
			save = no_son_expr(first_lexems, save);
	}
}
