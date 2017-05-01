#include "21sh.h"

char		**ftp_list_to_tabstr(t_list *list)
{
	t_list		*tmp;
	char		**args;
	uint32_t	size;
	uint32_t	i;

	tmp = list;
	size = ft_list_size(list);
	if (!(args = (char**)malloc(sizeof(char*) * (size + 1))))
		ftp_error(NULL, "ERROR: malloc failure\n", 0);
	args[size] = NULL;
	i = 0;
	while (tmp)
	{
		args[i++] = (char*)tmp->data;
		tmp = tmp->next;
	}
	return (args);
}

/*t_list		*ftp_tabstr_to_list(char **args)
{
	t_list	*list;
	t_arg	*arg;
	int		i;

	i = -1;
	list = NULL;
	while (args[++i])
	{
		if (!(arg = (t_arg*)malloc(sizeof(t_arg))))
			ftp_error(NULL, "ERROR: Malloc failure !", 0);
		if (args[i][ft_strlen(args[i]) - 1] == '/')
			args[i][ft_strlen(args[i]) - 1] = 0;
		arg->str = ft_strtrim(args[i]);
		arg->base = 1;
		ft_node_push_back(&list, arg);
	}
	return (list);
}*/
