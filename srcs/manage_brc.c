#include "tosh.h"

static void get_rest_expr(t_list **lexems, t_list **new_lexems)
{
    t_token *token;

    *lexems = (*lexems)->next;
    while (*lexems)
    {
        token = (t_token*)(*lexems)->data;
        if (TYPE == END_EXPR)
            break;
        ft_node_push_back(new_lexems, new_token(NULL, TYPE, VAL));
        *lexems = (*lexems)->next;
    }
}

static void get_expr_start(t_list *lexems, t_list **new_lexems, e_token type)
{
    t_token *token;

    while (lexems->prev)
    {
        token = (t_token*)lexems->prev->data;
        lexems = lexems->prev;
        if (TYPE == type)
            break;
    }
    while (lexems->prev)
    {
        token = (t_token*)lexems->prev->data;
        if (is_glob_token(TYPE) || TYPE == EXPR)
            ft_node_push_front(new_lexems, new_token(NULL, TYPE, VAL));
        else
            break;
        lexems = lexems->prev;
    }
}

static char create_expr_from_brc(t_list *lexems, t_token *token,
            t_list **nw_lexems)
{
    t_list  *tmp;
    
    if (TYPE != RBRC)
        get_expr_start(lexems, nw_lexems, LBRC);
    tmp = lexems;
    while (tmp && TYPE != RBRC && TYPE != END_EXPR)
    {
        tmp = tmp->next;
        token = (t_token*)tmp->data;
    }
    if (tmp->next && TYPE != END_EXPR)
     get_rest_expr(&tmp, nw_lexems);
    ft_node_push_back(nw_lexems, new_token(NULL, END_EXPR, ""));
    ft_add_list(tmp, *nw_lexems);
    token = (t_token*)lexems->data;
    if (TYPE == RBRC)
        return (0);
    *nw_lexems = NULL;
    return (1);
}

void        manage_brc(t_list *lexems)
{
    t_token *token;
    t_list  *new_lexems;

    new_lexems = NULL;
    while (lexems)
    {
        token = (t_token*)lexems->data;
        if (TYPE == END_EXPR)
            break;
        if (TYPE == COM || TYPE == RBRC)
        {
            if (!(create_expr_from_brc(lexems, token, &new_lexems)))
                break;
        }
        else
            ft_node_push_back(&new_lexems, new_token(NULL, TYPE, VAL));
        lexems = lexems->next;
    }
}