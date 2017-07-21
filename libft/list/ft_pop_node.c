#include "list.h"

void	ft_pop_node(t_list **node, void (*fct_free)(void *))
{
    t_list  *tmp;

	if (*node)
	{
        tmp = *node;
		if (tmp->next)
			tmp->next->prev = tmp->prev;
		if (tmp->prev)
			tmp->prev->next = tmp->next;
        else
            *node = tmp->next;
        if (fct_free)
		    (*fct_free)(tmp->data);
		free(tmp);
        tmp = NULL;
    }
}