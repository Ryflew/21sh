#include "tosh.h"
#include <dirent.h>
#include <sys/stat.h>

/*char        find_last_directory(t_sh *sh)
{
    t_list  *tmp;
    t_token *token;

    tmp = sh->lexer->lexems;
    while (tmp)
    {
        token = (t_token*)tmp->data;
        if (token->type == NONE || (token->type == EXPR && ft_strchr(token->value, '/')))
            break;
        tmp = tmp->next;
    }
    if (!tmp || token->type = NONE)
        return (0);
    else
        return (1);
}*/

void    clear_old_expr(t_sh *sh, t_list **lexems)
{
    t_token *token;
    t_list  *tmp;
    
    tmp = *lexems;
    while (*lexems)
    {
        token = (t_token*)(*lexems)->data;
        if (token->type == NONE)
        {
            ft_pop_node(lexems, (void*)&clear_lexems);                
            break;
        }
        ft_pop_node(lexems, (void*)&clear_lexems);                
    }
    if (!tmp->prev)
        sh->lexer->lexems = *lexems;
}

int			find_last_directory(char *path)
{
	int i;

	i = ft_strlen(path) - 1;
	while (i > 0 && path[i] != '/')
		--i;
	return (i);
}

DIR		*open_dir(char *dir_name, t_token *token)
{
    DIR				*dir;
    char            *dir_path;

    if (token->type != EXPR || token->value[0] != '/')
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

int		is_dir(char *dir_name, t_token *token)
{
	struct stat statbuf;
    char        *dir_path;
    
    if (token->type != EXPR || token->value[0] != '/')
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

char        is_end_path(t_list *lexems)
{
    t_token *token;

    while (lexems)
    {
        token = (t_token*)lexems->data;
        if (token->type == NONE)
            return (0);
        if (token->type == EXPR && ft_strchr(token->value, '/'))
            return (1);
        lexems = lexems->next;
    }
    return (0);
}

void        add_end_path(t_list *lexems, t_list ** new_lexems)
{
    t_token *token;
    t_token *nw_token;
    char    *to_free;
    char    copy;

    copy = 0;
    while (lexems)
    {
        token = (t_token*)lexems->data;
        if (token->type == NONE)
            break;
        if (token->type == EXPR && ft_strchr(token->value, '/'))
            ++copy;
        if (copy)
        {
            if (token->type == EXPR && ft_strchr(token->value, '/') && copy == 1)
            {
                nw_token = (t_token*)(*new_lexems)->data;
                to_free = nw_token->value;
                nw_token->value = ft_strjoin(nw_token->value, token->value + ft_strfind(token->value, 0, '/'));
                free(to_free);
            }
            else
                ft_node_push_back(new_lexems, new_token(NULL, token->type, token->value));
        }
        lexems = lexems->next;
    }
}

static void	fill_new_lexems(t_list *lexems, char end_path, char *join)
{
    t_list  *tmp;
    t_list  *new_lexems;

    new_lexems = NULL;
    ft_node_push_back(&new_lexems, new_token(NULL, EXPR, join));
    if (end_path)
        add_end_path(lexems->next, &new_lexems);
    ft_node_push_back(&new_lexems, new_token(NULL, NONE, ""));        
    tmp = lexems;
    while (tmp)
    {
        if (((t_token*)(tmp->data))->type == NONE)
            break;
        tmp = tmp->next;
    }
    ft_add_list(tmp, new_lexems);
    free(join);
}

void	    manage_wildcards(t_list *lexems)
{
	DIR				*dir;
	struct dirent	*ent;
    char			*dir_path;
    t_token         *token;
    char            *join;

    token = (t_token*)lexems->data;
    dir_path = ft_strsub(token->value, 0, find_last_directory(token->value));
	if ((dir = open_dir(dir_path, token)))
	{
		while ((ent = readdir(dir)) != NULL)
		{
            if (dir_path[0])
                join = ft_strstrjoin(dir_path, "/", ent->d_name);
            else
                join = ft_strdup(ent->d_name);
			if (ent->d_name[0] != '.' && (is_dir(join, token) || !is_end_path(lexems->next))
			&& ((token->type != EXPR && nmatch(ent->d_name, NULL, lexems)) || (token->type == EXPR && ((dir_path[0] && nmatch(ent->d_name, token->value + find_last_directory(token->value) + 1, lexems)) || (!dir_path[0] && nmatch(ent->d_name, token->value, lexems))))))
				fill_new_lexems(lexems, is_end_path(lexems->next), join);
		}
		closedir(dir);
	}
	free(dir_path);
}

void        search_expr(t_list **lexems)
{
    t_token *token;
    t_list  *save;

    save = NULL;
    while (*lexems)
    {
        token = (t_token*)(*lexems)->data;
        if (token->type == NONE)
            save = NULL;
        else
        {
            if (!save && (is_glob_token(token->type) || token->type == EXPR))
                save = *lexems;
            if (token->type == S_WILDCARD || token->type == Q_WILDCARD || token->type == E_WILDCARD
            || token->type == LBKT || token->type == RBKT || token->type == LBRC)
            {
                *lexems = save;
                break;
            }
        }
        *lexems = (*lexems)->next;
    }
}

void		expr_loop(t_sh *sh)
{
    t_list	*lexems;

	lexems = sh->lexer->lexems;
    search_expr(&lexems);
    while (lexems)
    {
        manage_wildcards(lexems);
        clear_old_expr(sh, &lexems);
        search_expr(&lexems);
    }
}

void    get_rest_expr(t_list **lexems, t_list **new_lexems)
{
    t_token *token;

    *lexems = (*lexems)->next;
    while (*lexems)
    {
        token = (t_token*)(*lexems)->data;
        if (token->type == NONE)
            break;
        ft_node_push_back(new_lexems, new_token(NULL, token->type, token->value));            
        *lexems = (*lexems)->next;
    }
}

void    get_expr_start(t_list *lexems, t_list **new_lexems, e_token type)
{
    t_token *token;

    while (lexems->prev)
    {
        token = (t_token*)lexems->prev->data; 
        lexems = lexems->prev;
        if (token->type == type)
            break;   
    }
    while (lexems->prev)
    {
        token = (t_token*)lexems->prev->data;
        if (is_glob_token(token->type) || token->type == EXPR)
            ft_node_push_front(new_lexems, new_token(NULL, token->type, token->value));            
        else
            break;
        lexems = lexems->prev;
    }
}

void    manage_brc(t_list *lexems)
{
    t_token *token;
    t_list  *new_lexems;
    t_list  *tmp;

    new_lexems = NULL;
    while (lexems)
    {
        token = (t_token*)lexems->data;
        if (token->type == NONE)
            break;
        if (token->type == COM || token->type == RBRC)
        {
            if (token->type != RBRC)
                get_expr_start(lexems, &new_lexems, LBRC);            
            tmp = lexems;
            while (tmp && token->type != RBRC && token->type != NONE)
            {
                tmp = tmp->next;
                token = (t_token*)tmp->data;
            }
            if (tmp->next && token->type != NONE)
                get_rest_expr(&tmp, &new_lexems);
            ft_node_push_back(&new_lexems, new_token(NULL, NONE, ""));
            ft_add_list(tmp, new_lexems);
            token = (t_token*)lexems->data;        
            if (token->type == RBRC)
                break;
            new_lexems = NULL;
        }
        else
            ft_node_push_back(&new_lexems, new_token(NULL, token->type, token->value));        
        lexems = lexems->next;
    }
}

void    merge_expr(t_list *lexems)
{
    t_token *token;
    t_token *next_token;
    char    *to_free;

    while (lexems->next)
    {
        token = (t_token*)lexems->data;
        next_token = (t_token*)lexems->next->data;
        if (token->type == EXPR && (!lexems->prev || !is_glob_token(((t_token*)lexems->prev->data)->type)) && next_token->type == NONE)
            token->type = WORD;
        if ((token->type == EXPR || token->type == BKT_EXPR) && (next_token->type == EXPR || next_token->type == BKT_EXPR))
        {
            to_free = token->value;
            token->value = ft_strjoin(token->value, next_token->value);
            free(to_free);
            ft_pop_node(&(lexems->next), (void*)&clear_lexems);
        }
        else
            lexems = lexems->next;
    }
}

void    merge_expr_to_word(t_list *lexems)
{
    t_token *token;
    t_token *next_token;
    char    *to_free;

    while (lexems->next)
    {
        token = (t_token*)lexems->data;
        next_token = (t_token*)lexems->next->data;
        if (next_token->type == NONE)
        {
            token->type = WORD;
            ft_pop_node(&(lexems->next), (void*)&clear_lexems);
            break;
        }
        to_free = token->value;
        token->value = ft_strjoin(token->value, next_token->value);
        free(to_free);
        ft_pop_node(&(lexems->next), (void*)&clear_lexems);
    }
}

void    clear_bad_expr(t_list *lexems)
{
    t_token *token;
    t_list  *save;
    char    brc;
    char    bkt;
    char    pb;
 
    save = NULL;
    brc = 0;
    bkt = 0;
    pb = 0;
    while (lexems)
    {
        token = (t_token*)lexems->data;
        if (!save && (is_glob_token(token->type) || token->type == EXPR))
            save = lexems;
        else if (token->type == NONE)
        {
            if (brc || bkt || pb)
                merge_expr_to_word(save);
            save = NULL;
        }
        if (token->type == LBRC)
        {
            if (brc || bkt || ((t_token*)(lexems->next->data))->type == RBRC)
                 pb = 1;
            brc = 1;
        }
        else if (token->type == COM && (bkt || !brc))
            pb = 1;
        else if (token->type == RBRC)
        {
            if (bkt || !brc)
                pb = 1;
            brc = 0;
        }
        else if (token->type == LBKT || token->type == E_WILDCARD)
        {
            if (bkt || ((t_token*)(lexems->next->data))->type == RBKT)
                pb = 1;
            bkt = 1;
        }
        else if (token->type == RBKT)
        {
            if (!bkt)
                pb = 1;
            bkt = 0;
        }
        lexems = lexems->next;        
    }
}

void    replace_expr_by_word(t_list *lexems)
{
    t_token *token;

    while (lexems)
    {
        token = (t_token*)lexems->data;
        if (token->type == NONE)
            ft_pop_node(&lexems, (void*)&clear_lexems);
        else
        {
            if (token->type == EXPR)
                token->type = WORD;
            lexems = lexems->next;
        }
    }
}

void    glob(t_sh *sh)
{
    t_list  *lexems;
    t_token *token;

    lexems = sh->lexer->lexems;
    clear_bad_expr(lexems);
    while (lexems)
    {
        token = (t_token*)lexems->data;
        if (token->type == LBRC)
        {
            manage_brc(lexems->next);
            clear_old_expr(sh, &lexems);
            merge_expr(sh->lexer->lexems);  
        }
        else
            lexems = lexems->next;   
    }
  /*  t_list *tmp = sh->lexer->lexems;    
	while (tmp)
	{
		ft_putnbr(((t_token*)(tmp->data))->type);
		ft_putendl(" <=== type");
		ft_putendl(((t_token*)(tmp->data))->value);
		tmp = tmp->next;
	}*/
    expr_loop(sh);
    replace_expr_by_word(sh->lexer->lexems);
}