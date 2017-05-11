#include <stdlib.h>
#include "21sh.h"

void		errexit(char *content, char *reason)
{
	ft_fputstr(content, 2);
	ft_fputstr(": ", 2);
	ft_fputendl(reason, 2);
}

void		exit_command(char **av, t_sh *shell)
{
	int		i;

	if (!*++av)
	{
		save_history(shell);
		tcsetattr(0, TCSADRAIN, &(shell->old));
		exit(EXIT_SUCCESS);
	}
	i = 0;
	while (*av + i && ((*(*av + i) >= '0' && *(*av + i) <= '9') || \
			*(*av + i) == '-'))
		++i;
	if (!*(*av + i) && !*(av + 1))
	{
		tcsetattr(0, TCSADRAIN, &(shell->old));
		save_history(shell);
		exit(ft_atoi(*av));
	}
	else if ((**av >= '0' && **av <= '9') && *(*av + i))
		ft_putstr("Badly formed number.");
	else if (*(av + 1) || (*(*av + i) < '0' || *(*av + i) > '9' || \
			*(*av + i) != '-'))
		ft_putstr("Expression Syntax.");
}
