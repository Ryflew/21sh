#include "list.h"

void	ft_pop_node(t_list **node, void (*fct_free)(void *))
{
	t_list  *tmp;

	if (*node)
	{
		tmp = *node;
		if (tmp->prev)
		{
			*node = tmp->prev;
			tmp->prev->next = tmp->next;
		}
		if (tmp->next)
		{
			*node = tmp->next;			
			tmp->next->prev = tmp->prev;
		}
		if (fct_free)
			(*fct_free)(tmp->data);
		free(tmp);
		if (tmp == *node)
			*node = NULL;
		tmp = NULL;
	}
}
